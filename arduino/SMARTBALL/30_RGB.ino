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
  blinkLed(RED, LONG_BLINK);
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

void updateStrobe() {
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
  return color(values, 0);
}

int32_t color(byte* data, uint16_t addr) {
  return (data[addr++] << 16) | (data[addr++] << 8) | data[addr];
}

uint32_t blinkLed(uint32_t c, uint16_t BLINK_DUR) {
  if (bli) {
    colors[0][1] = c;
    updateRGB();
    delay(BLINK_DUR);
    colors[0][1] = BLACK;
    updateRGB();
    delay(BLINK_GAP);
    return BLINK_DUR + BLINK_GAP;
  }
  delay(1);
  return 1;
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void setRGB(byte slot, byte n, uint16_t addr) {
  int32_t c1, c2, c3;
  switch (n) {
    default:
      for (int i = 0; i < RGB_NUM; i++)
        colors[slot][i] = color(_DIN, addr);
      break;
    case 2:
      for (int i = 0; i < RGB_NUM / 2; i++)
        colors[slot][i] = color(_DIN, addr);
      for (int i = RGB_NUM / 2; i < RGB_NUM; i++)
        colors[slot][i] = color(_DIN, addr + 3);
      break;
    case 3:
      c1 = color(_DIN, addr);
      c2 = color(_DIN, addr + 3);
      c3 = color(_DIN, addr + 6);
      colors[slot][0] = c1;
      colors[slot][5] = c1;
      colors[slot][1] = c2;
      colors[slot][4] = c2;
      colors[slot][2] = c3;
      colors[slot][3] = c3;
      break;
    case 6:
      for (int i = 0; i < RGB_NUM; i++)
        colors[slot][i] = color(_DIN, addr + 3 * i);
      break;
  }
}

void setSTB(uint16_t addr) {
  strobeSpeed = (_DIN[addr] << 8) | _DIN[addr + 1];                       // in 0.01 ms steps
  strobeSpeed = strobeSpeed ? constrain(strobeSpeed, 1000, 50000) : 0;    // no strobe over 50 Hz and under 1 Hz
  strobeSpeed *= 10;                                                      // transformation into µs
}

void setMST(uint16_t addr) {
  dimmer = (float)((_DIN[addr] << 8) | _DIN[addr + 1]) / 100.;            // Q14.2
  dimmer = constrain(dimmer, 0., 100.);                                   // dimmer contrain from 0. to 100.
}
