//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

int imuCounter = 0;
MPU9250 IMU(SPI, IMU_CS);

// FILTERS IMPLEMENTATION
byte  ballState;
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

  // DENIS FREE FALL FILTER
  ballState = ((IMU.getAccelN_mss() > 15.) << 1) | ((IMU.getAccelN_mss() < 8.) << 0) ;

  if (imuCounter == 0) {
    if (dgm) sendIMU();
    if (osc) sendOSCIMU();
  }
  imuCounter++;
  imuCounter %= 3;
}

//-----------------------------------------------------------------------------------
// DATAGRAM FEEDBACK
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
      val = ballState;
      _DOUT[index++] = val >> 8; _DOUT[index++] = val & 255;
    }

    sendDgmAnswer(CMD_IMU, index);
  }
}


//-----------------------------------------------------------------------------------
// OSC FEEDBACK
//-----------------------------------------------------------------------------------

void sendOSCIMU() {
  OSCMessage msg("/sb/imu");
  msg.add(IMU.getAccelX_mss());
  msg.add(IMU.getAccelY_mss());
  msg.add(IMU.getAccelZ_mss());
  msg.add(IMU.getGyroX_rads());
  msg.add(IMU.getGyroY_rads());
  msg.add(IMU.getGyroZ_rads());
  msg.add(IMU.getMagX_uT());
  msg.add(IMU.getMagY_uT());
  msg.add(IMU.getMagZ_uT());
  OSC.beginPacket(oset.outputIp, oset.oscOutputPort);
  msg.send(OSC);
  OSC.endPacket();
  msg.empty();
}
