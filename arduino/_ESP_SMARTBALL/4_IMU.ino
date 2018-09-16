//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

MPU9250 IMU(SPI, IMU_CS);
bool changeAR = false;
bool changeGR = false;

//---------------------------------------------------------------
// BASIC FUNCTIONS
//---------------------------------------------------------------

bool imuAvailable() {
  return factorySettings.deviceFlag & (1 << IMU_BIT);
}
void initIMU() {
  strip.updatePins();
  IMU.begin();
  IMU.setAccelRange((AccelRange)imuSettings.accRange);
  IMU.setGyroRange((GyroRange)imuSettings.gyrRange);
  strip.updatePins(D7, D5);
}

void updateIMU() {
  strip.updatePins();
  if (changeAR) {
    IMU.setAccelRange((AccelRange)imuSettings.accRange);
    changeAR = false;
  }
  if (changeGR) {
    IMU.setGyroRange((GyroRange)imuSettings.gyrRange);
    changeGR = false;
  }
  IMU.readSensor();
  strip.updatePins(D7, D5);

  // data filter (a low pass filter probably)
  // data computation (for flags, peak, detection, apogee prediction)
  if (imuSettings.streamFlag) sendIMU();
}

//---------------------------------------------------------------
// PROTOCOL FUNCTIONS
//---------------------------------------------------------------

void setIMU() {
  imuSettings.streamFlag = _DIN[0];
}

void setAccRange() {
  imuSettings.accRange = _DIN[0];
  changeAR = true;
}

void setGyrRange() {
  imuSettings.gyrRange = _DIN[0];
  changeGR = true;
}

void saveImuSettings() {
  EEPROM.begin(512);
  EEPROM.put(IMU_SETTINGS_ADDR, imuSettings);
  EEPROM.end();
}

// SENDING DATAS -------------------------------------------------
void sendIMU() {
  int16_t  val = 0;
  uint16_t index = 0;
  _DOUT[index++] = imuSettings.streamFlag;
  if (imuSettings.streamFlag & (1 << ACC_BIT)) {
    val = (int16_t)(IMU.getAccelX_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getAccelY_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getAccelZ_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }
  if (imuSettings.streamFlag & (1 << GYR_BIT)) {
    val = (int16_t)(IMU.getGyroX_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getGyroY_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getGyroZ_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }
  if (imuSettings.streamFlag & (1 << MAG_BIT)) {
    val = (int16_t)(IMU.getMagX_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getMagY_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getMagZ_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }
  if (imuSettings.streamFlag & (1 << TMP_BIT)) {
    val = (int16_t)(IMU.getTemperature_C() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }
  sendDgmAnswer(CMD_IMU,index);
}
