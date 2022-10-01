//******************************************************************************************
// IMU Diagnostic functions
//******************************************************************************************

void getMAC() {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

//******************************************************************************************
// IMU Diagnostic functions
//******************************************************************************************

void initIMU() {
  IMU.begin();
}

void testIMU() {
  IMU.readSensor();
  Serial.print("ACC (xyzn): ");
  Serial.print(IMU.getAccelX_mss(), 2);
  Serial.print(", ");
  Serial.print(IMU.getAccelY_mss(), 2);
  Serial.print(", ");
  Serial.print(IMU.getAccelZ_mss(), 2);
  Serial.print(", ");
  Serial.print(IMU.getAccelN_mss(), 2);
  Serial.print("\t");
  Serial.print("GYR (xyzn): ");
  Serial.print(IMU.getGyroX_rads(), 2);
  Serial.print(", ");
  Serial.print(IMU.getGyroY_rads(), 2);
  Serial.print(", ");
  Serial.print(IMU.getGyroZ_rads(), 2);
  Serial.print(", ");
  Serial.print(IMU.getGyroN_rads(), 2);
  Serial.print("\t");
  Serial.print("MAG (xyz): ");
  Serial.print(IMU.getMagX_uT(), 2);
  Serial.print(", ");
  Serial.print(IMU.getMagY_uT(), 2);
  Serial.print(", ");
  Serial.print(IMU.getMagZ_uT(), 2);
  Serial.print("\t");
  Serial.print("TEMP: ");
  Serial.println(IMU.getTemperature_C(), 2);
}

//******************************************************************************************
// RGB LEDS diagnostic functions
//******************************************************************************************

void initLED() {
  strip.begin();
  pinMode(RGB_CS, OUTPUT);
  digitalWrite(RGB_CS, HIGH);
  strip.setStartPixel(0b11111);
  changeAll(0x0);
}

void testLED() {
  changeAll(0x200000);
  delay(SPEED);
  changeAll(0x002000);
  delay(SPEED);
  changeAll(0x000020);
  delay(SPEED);
  changeAll(0x000000);
}

void fullLED() {
  changeAll(0x202020);
}

void changeAll(int32_t color) {
  for (int i = 0; i < NUM_LED; i++) {
    strip.setPixelColor(i, color);
  }
  strip.updatePins(D7, D5);
  digitalWrite(RGB_CS, LOW);
  strip.show();
  digitalWrite(RGB_CS, HIGH);
  strip.updatePins();
}

//******************************************************************************************
// VIBRATION MOTOR diagnostic functions
//******************************************************************************************

void initMOT() {
  pinMode(MOT_PIN, OUTPUT);
}

void testMOT() {
  analogWrite(MOT_PIN, VALUE);
  delay(SPEED);
  analogWrite(MOT_PIN, 0);
  delay(SPEED);
}

//******************************************************************************************
// INFRARED LEDS diagnostic functions
//******************************************************************************************

void initIRL() {
  pinMode(IRL_PIN, OUTPUT);
}

void testIRL() {
  analogWrite(IRL_PIN, VALUE);
  delay(SPEED);
  analogWrite(IRL_PIN, 0);
  delay(SPEED);
}

//******************************************************************************************
// VIBRATION MOTOR diagnostic functions
//******************************************************************************************

void initBUZ() {
  #ifdef BUZ_PIN
  pinMode(BUZ_PIN, OUTPUT);
  #endif
}

void testBUZ() {
  #ifdef BUZ_PIN
  tone(BUZ_PIN, FREQ);
  delay(SPEED);
  noTone(BUZ_PIN);
  delay(SPEED);
  #endif
}
