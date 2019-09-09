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

void initIMU() {
  strip.updatePins();
  IMU.begin();
  strip.updatePins(D7, D5);
}

void updateIMU() {
  // VALUE BACKUP
  float prevAccelN  = IMU.getAccelN_mss();
  float prevAccelD  = accFirstDerivative;
  float prevGyroN   = IMU.getGyroN_rads();
  float prevGyroD   = gyrFirstDerivative;
  previousFreeFallFilter = freeFallFilter;

  // SENSOR READING
  strip.updatePins();
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
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void sendIMU() {
  if (gset.imuFlag) {
    int16_t  val = 0;
    uint16_t index = 0;
    _DOUT[index++] = gset.imuFlag;

    if (gset.imuFlag & (1 << ACC_BIT)) {
      val = (int16_t)(IMU.getAccelX_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getAccelY_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getAccelZ_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << GYR_BIT)) {
      val = (int16_t)(IMU.getGyroX_rads() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getGyroY_rads() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getGyroZ_rads() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << MAG_BIT)) {
      val = (int16_t)(IMU.getMagX_uT() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getMagY_uT() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getMagZ_uT() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << TMP_BIT)) {
      val = (int16_t)(IMU.getTemperature_C() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << VEC_BIT)) {
      val = (int16_t)(IMU.getAccelN_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getGyroN_rads() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getMagN_uT() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << QUA_BIT)) {
      val = (int16_t)(IMU.getQuatW() * 10000.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getQuatX() * 10000.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getQuatY() * 10000.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getQuatZ() * 10000.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << WLD_BIT)) {
      val = (int16_t)(IMU.getWorldX_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getWorldY_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
      val = (int16_t)(IMU.getWorldZ_mss() * 100.);
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    if (gset.imuFlag & (1 << STA_BIT)) {
      val = freeFallState;
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    sendDgmAnswer(CMD_IMU, index);
  }
}
