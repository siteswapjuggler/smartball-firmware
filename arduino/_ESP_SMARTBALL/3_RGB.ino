//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

Adafruit_DotStar strip = Adafruit_DotStar(RGB_NUM, D7, D5, DOTSTAR_BGR);

//---------------------------------------------------------------
// BASIC FUNCTIONS
//---------------------------------------------------------------

void initRGB() {
  pinMode(RGB_CS, OUTPUT);
  digitalWrite(RGB_CS, LOW);
  strip.begin();
  digitalWrite(RGB_CS, HIGH);
  blinkRGB(RED, 500);
}

void updateRGB() {
  digitalWrite(RGB_CS, LOW);
  strip.show();
  digitalWrite(RGB_CS, HIGH);
}

//---------------------------------------------------------------
// PROTOCOL FUNCTIONS
//---------------------------------------------------------------

void setRGB(byte n, uint16_t offset) {
  int32_t c1, c2, c3;
  switch (n) {
    default:
      changeRGB(rgb(_DIN, 0 + offset));
      break;
    case 2:
      for (int i = 0; i < RGB_NUM / 2; i++)
        strip.setPixelColor(i, rgb(_DIN, 0 + offset));
      for (int i = RGB_NUM / 2; i < RGB_NUM; i++)
        strip.setPixelColor(i, rgb(_DIN, 3 + offset));
      break;
    case 3:
      c1 = rgb(_DIN, 0 + offset);
      c2 = rgb(_DIN, 3 + offset);
      c3 = rgb(_DIN, 6 + offset);
      strip.setPixelColor(0, c1);
      strip.setPixelColor(5, c1);
      strip.setPixelColor(1, c2);
      strip.setPixelColor(4, c2);
      strip.setPixelColor(2, c3);
      strip.setPixelColor(3, c3);
      break;
    case 6:
      for (int i = 0; i < RGB_NUM; i++)
        strip.setPixelColor(i, rgb(_DIN, 3 * i + offset));
      break;
  }
}

void setSTM(byte ballNumber) {
  byte colorNumber = _DIN[0];
  setRGB(colorNumber, 1 + 3 * colorNumber * (factorySettings.serialNumber % ballNumber));
}

bool rgbAvailable() {
  return factorySettings.deviceFlag & (1 << RGB_BIT);
}

//---------------------------------------------------------------
// OTHER FUNCTIONS
//---------------------------------------------------------------

int32_t rgb(byte * data, byte addr) {
  return (data[addr++] << 16) | (data[addr++] << 8) | data[addr];
}

void changeRGB(int32_t c) {
  for (int i = 0; i < RGB_NUM; i++)
    strip.setPixelColor(i, c);
}

void blinkRGB(uint32_t c, unsigned int dur) {
  changeRGB(c);
  updateRGB();
  delay(dur);
  changeRGB(BLACK);
  updateRGB();
}
