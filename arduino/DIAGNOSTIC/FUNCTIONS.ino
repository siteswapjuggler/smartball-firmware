//******************************************************************************************
// IMU Diagnostic functions
//******************************************************************************************

void initIMU() {
  IMU.begin();
}

void testIMU() {
  IMU.readSensor();
  Serial.print(IMU.getAccelX_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(), 2);
  Serial.print("\t");
  Serial.println(IMU.getTemperature_C(), 2);
}

//******************************************************************************************
// RGB LEDS diagnostic functions
//******************************************************************************************

void initLED() {
  strip.begin();
  pinMode(LED_CS, OUTPUT);
  digitalWrite(LED_CS, HIGH);
  changeAll(0x0);
}

void testLED() {
  changeAll(0xFF0000);
  delay(SPEED);
  changeAll(0x00FF00);
  delay(SPEED);
  changeAll(0x0000FF);
  delay(SPEED);
  changeAll(0x000000);
}

void changeAll(int32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.updatePins(D7, D5);
  digitalWrite(LED_CS, LOW);
  strip.show();
  digitalWrite(LED_CS, HIGH);
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
