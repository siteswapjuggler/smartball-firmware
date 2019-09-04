//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

MPU9250 IMU(SPI, IMU_CS);
bool changeAR = false;
bool changeGR = false;

// FILTERS IMPLEMENTATION

byte  freeFallState, freeFallFilter = 0., previousFreeFallFilter;
float accFirstDerivative = 0., accSecondDerivative;
float gyrFirstDerivative = 0., gyrSecondDerivative;

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
  
  // VALUE BACKUP
  float prevAccelN  = IMU.getAccelN_mss();
  float prevAccelD  = accFirstDerivative;
  float prevGyroN   = IMU.getGyroN_rads();
  float prevGyroD   = gyrFirstDerivative;
  previousFreeFallFilter = freeFallFilter;
  
  // SENSOR READING
  IMU.readSensor();
  strip.updatePins(D7, D5);
  
  // MADGWICK FILTER
  IMU.madgwickUpdate();
  IMU.realWorldUpdate();

  //**************************************************************************************
  // WIP data computation (flags, peak, detection, apogee prediction)
  //**************************************************************************************

  // FIRST & SECOND DERIVATIVE
  accFirstDerivative  = IMU.getAccelN_mss() - prevAccelN;
  accSecondDerivative = accFirstDerivative - prevAccelD;
  gyrFirstDerivative  = IMU.getGyroN_rads() - prevGyroN;
  gyrSecondDerivative = gyrFirstDerivative - prevGyroD;
  
  // FREE FALL FILTER
 
  freeFallFilter  = (IMU.getGyroN_rads() > 8.72665) ? abs(gyrFirstDerivative) < 0.0898132 : abs(gyrFirstDerivative) < 0.0174533; 
  freeFallFilter  = freeFallFilter | (abs(accFirstDerivative) < 0.1) << 1 | (abs(accSecondDerivative) < 0.25) << 2 | (IMU.getGyroN_rads() > 0.872665) << 3;
  freeFallState   = (previousFreeFallFilter + freeFallFilter) > 1;


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

  if (iset.streamFlag & (1 << TMP_BIT)) {
    val = (int16_t)(IMU.getTemperature_C() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  // NOUVEAUTÉ -------------------------------------------

  if (iset.streamFlag & (1 << VEC_BIT)) {
    val = (int16_t)(IMU.getAccelN_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getGyroN_rads() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getMagN_uT() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << QUA_BIT)) {
    val = (int16_t)(IMU.getQuatW() * 10000.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getQuatX() * 10000.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getQuatY() * 10000.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getQuatZ() * 10000.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << WLD_BIT)) {
    val = (int16_t)(IMU.getWorldX_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getWorldY_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    val = (int16_t)(IMU.getWorldZ_mss() * 100.);
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  if (iset.streamFlag & (1 << STA_BIT)) {
    val = freeFallState;//0b1010101010101010;
    _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
  }

  // NOUVEAUTÉ -------------------------------------------

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
