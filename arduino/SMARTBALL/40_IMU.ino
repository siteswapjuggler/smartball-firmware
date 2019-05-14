//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

MPU9250 IMU(SPI, IMU_CS);
bool changeAR = false;
bool changeGR = false;
float accMagnitude, gyrMagnitude, magMagnitude;

//-----------------------------------------------------------------------------------
// BASIC FUNCTIONS
//-----------------------------------------------------------------------------------

bool imuAvailable() {
  return fset.deviceFlag & (1 << IMU_BIT);
}
void initIMU() {
  strip.updatePins();
  IMU.begin();
  IMU.setAccelRange((AccelRange)iset.accRange);
  IMU.setGyroRange((GyroRange)iset.gyrRange);
  strip.updatePins(D7, D5);
}

void updateIMU() {
  strip.updatePins();
  if (changeAR) {
    IMU.setAccelRange((AccelRange)iset.accRange);
    changeAR = false;
  }
  if (changeGR) {
    IMU.setGyroRange((GyroRange)iset.gyrRange);
    changeGR = false;
  }

  // SENSOR READING
  IMU.readSensor();
  strip.updatePins(D7, D5);

  // MAGNITUDE UPDATE
  accMagnitude = sqrt(IMU.getAccelX_mss() * IMU.getAccelX_mss() + IMU.getAccelY_mss() * IMU.getAccelY_mss() + IMU.getAccelZ_mss() * IMU.getAccelZ_mss());
  gyrMagnitude = sqrt(IMU.getGyroX_rads() * IMU.getGyroX_rads() + IMU.getGyroY_rads() * IMU.getGyroY_rads() + IMU.getGyroZ_rads() * IMU.getGyroZ_rads());
  magMagnitude = sqrt(IMU.getMagX_uT() * IMU.getMagX_uT() + IMU.getMagY_uT() * IMU.getMagY_uT() + IMU.getMagZ_uT() * IMU.getMagZ_uT());

  // QUATERNION FILTERS
  //IMU.quaternionUpdate();
  //IMU.realWorldAccel();

  // TODO data filter (a low pass filter probably)
  // TODO data computation (for flags, peak, detection, apogee prediction)

  if (iset.streamFlag) sendIMU();
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void setIMU() {
  iset.streamFlag = _DIN[0];
}

void setAccRange() {
  iset.accRange = _DIN[0];
  changeAR = true;
}

void setGyrRange() {
  iset.gyrRange = _DIN[0];
  changeGR = true;
}

void sendIMU() {
  int16_t  val = 0;
  uint16_t index = 0;
  _DOUT[index++] = iset.streamFlag;

  if (iset.streamFlag & (1 << ACC_BIT)) {
    val = (int16_t)(IMU.getAccelX_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getAccelY_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getAccelZ_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << GYR_BIT)) {
    val = (int16_t)(IMU.getGyroX_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getGyroY_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getGyroZ_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << MAG_BIT)) {
    val = (int16_t)(IMU.getMagX_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getMagY_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getMagZ_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << VEC_BIT)) {
    val = (int16_t)(accMagnitude * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(gyrMagnitude * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(magMagnitude * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << QUA_BIT)) {
    val = (int16_t)(10. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(20. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(30. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << WLD_BIT)) {
    val = (int16_t)(10. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(20. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(30. * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << STA_BIT)) {
    val = 0b1010101010101010;
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  sendDgmAnswer(CMD_IMU, index);
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveImuSettings() {
  EEPROM.begin(512);
  EEPROM.put(IS_ADDR, iset);
  EEPROM.end();
}
