//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

Adafruit_DotStar strip = Adafruit_DotStar(RGB_NUM, D7, D5, DOTSTAR_BGR);

byte strobeState = 0;
float dimmer = 100.;
int32_t colors[2][RGB_NUM] = {0};

//-----------------------------------------------------------------------------------
// BASIC FUNCTIONS
//-----------------------------------------------------------------------------------

void initRGB() {
  pinMode(RGB_CS, OUTPUT);
  digitalWrite(RGB_CS, LOW);
  strip.begin();
  digitalWrite(RGB_CS, HIGH);
  blinkRGB(RED, 500, 100);
}

void updateRGB() {
  digitalWrite(RGB_CS, LOW);
  for (int i = 0; i < RGB_NUM; i++) {
    strip.setPixelColor(i, dim(dimmer, colors[strobeState][i]));
  }
  strip.show();
  digitalWrite(RGB_CS, HIGH);
}

//-----------------------------------------------------------------------------------
// STROBE FUNCTIONS
//-----------------------------------------------------------------------------------

unsigned long strobeSpeed = 0.;
unsigned long previousTime = 0;
unsigned long strobeInterval = 0;

void strobeUpdate() {
  if (strobeSpeed) {
    unsigned long t = micros();
    if (t - previousTime >= strobeInterval) {
      strobeState = (strobeState + 1) % 2;
      updateRGB();
      previousTime = t;
    }
  }
}

void updateSTB() {
  strobeInterval = strobeSpeed;
  if (!strobeSpeed) {
    strobeState = 0;
  }
}

//-----------------------------------------------------------------------------------
// COLOR FUNCTIONS
//-----------------------------------------------------------------------------------

int32_t dim(float v, int32_t c) {
  byte values[] = {(c >> 16) & 255, (c >> 8) & 255, c & 255};
  for (byte i = 0; i < 3; i++) {
    float val = (float)values[i] * v / 100.;
    values[i] = round(val);
  }
  return rgb(values, 0);
}

int32_t rgb(byte* data, uint16_t addr) {
  return (data[addr++] << 16) | (data[addr++] << 8) | data[addr];
}

void changeRGB(byte slot, int32_t c) {
  for (int i = 0; i < RGB_NUM; i++)
    colors[slot][i] = c;
}

void blinkRGB(uint32_t c, uint16_t dur, uint16_t gap) {
  changeRGB(0, c);
  updateRGB();
  delay(dur);
  if (gap) {
    changeRGB(0, BLACK);
    updateRGB();
    delay(gap);
  }
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void setRGB(byte slot, byte n, uint16_t addr) {
  int32_t c1, c2, c3;
  switch (n) {
    default:
      changeRGB(slot, rgb(_DIN, addr));
      break;
    case 2:
      for (int i = 0; i < RGB_NUM / 2; i++)
        colors[slot][i] = rgb(_DIN, addr);
      for (int i = RGB_NUM / 2; i < RGB_NUM; i++)
        colors[slot][i] = rgb(_DIN, addr+3);
      break;
    case 3:
      c1 = rgb(_DIN, addr);
      c2 = rgb(_DIN, addr+3);
      c3 = rgb(_DIN, addr+6);
      colors[slot][0] = c1;
      colors[slot][5] = c1;
      colors[slot][1] = c2;
      colors[slot][4] = c2;
      colors[slot][2] = c3;
      colors[slot][3] = c3;
      break;
    case 6:
      for (int i = 0; i < RGB_NUM; i++)
        colors[slot][i] = rgb(_DIN, addr + 3*i);
      break;
  }
}

void setSTB(uint16_t addr) {
  strobeSpeed = (_DIN[addr] << 8) | _DIN[addr+1];                         // in 0.01 ms steps
  strobeSpeed = strobeSpeed ? constrain(strobeSpeed, 1000, 50000) : 0;    // no strobe over 50 Hz and under 1 Hz
  strobeSpeed *= 10;                                                      // transformation into µs
}

void setMST(uint16_t addr) {
  dimmer = (float)((_DIN[addr] << 8) | _DIN[addr+1])/100.;                // Q14.2
  dimmer = constrain(dimmer,0.,100.);                                     // dimmer contrain from 0. to 100.
}

bool rgbAvailable() {
  return fset.deviceFlag & (1 << RGB_BIT);
}
