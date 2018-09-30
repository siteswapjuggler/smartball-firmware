//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

Adafruit_DotStar strip = Adafruit_DotStar(RGB_NUM, D7, D5, DOTSTAR_BGR);

byte dimmer = 255;
byte strobeState = 0;
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

byte strobeSpeed = 0;
byte strobeInterval = 0;
unsigned long previousTime;

void strobeUpdate() {
  if (strobeSpeed) {
    unsigned long t = millis();
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

int32_t dim(byte v, int32_t c) {
  byte values[] = {(c >> 16) & 255, (c >> 8) & 255, c & 255};
  for (byte i = 0; i < 3; i++) {
    float val = (float)values[i] * (float)v / 255.;
    values[i] = round(val);
  }
  return rgb(values, 0);
}

int32_t rgb(byte* data, byte addr) {
  return (data[addr++] << 16) | (data[addr++] << 8) | data[addr];
}

void changeRGB(byte slot, int32_t c) {
  for (int i = 0; i < RGB_NUM; i++)
    colors[slot][i] = c;
}

void blinkRGB(uint32_t c, unsigned int dur, unsigned int gap) {
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

void setRGB(byte slot, byte n, uint16_t offsetet) {
  int32_t c1, c2, c3;
  switch (n) {
    default:
      changeRGB(slot, rgb(_DIN, 0 + offsetet));
      break;
    case 2:
      for (int i = 0; i < RGB_NUM / 2; i++)
        colors[slot][i] = rgb(_DIN, 0 + offsetet);
      for (int i = RGB_NUM / 2; i < RGB_NUM; i++)
        colors[slot][i] = rgb(_DIN, 3 + offsetet);
      break;
    case 3:
      c1 = rgb(_DIN, 0 + offsetet);
      c2 = rgb(_DIN, 3 + offsetet);
      c3 = rgb(_DIN, 6 + offsetet);
      colors[slot][0] = c1;
      colors[slot][5] = c1;
      colors[slot][1] = c2;
      colors[slot][4] = c2;
      colors[slot][2] = c3;
      colors[slot][3] = c3;
      break;
    case 6:
      for (int i = 0; i < RGB_NUM; i++)
        colors[slot][i] = rgb(_DIN, 3 * i + offsetet);
      break;
  }
}

void setSTM(byte ballNumber) {
  byte colorNumber = _DIN[0];
  setRGB(0, colorNumber, 1 + 3 * colorNumber * (fset.serialNumber % ballNumber));
}

void setSTB() {
  strobeSpeed = _DIN[0] ? constrain(_DIN[0], 10, 255) : 0;
}

void setMST() {
  dimmer = _DIN[0];
}

bool rgbAvailable() {
  return fset.deviceFlag & (1 << RGB_BIT);
}
