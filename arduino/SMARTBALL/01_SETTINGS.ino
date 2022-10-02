//-----------------------------------------------------------------------------------
// GLOBAL BUFFERS
//-----------------------------------------------------------------------------------

static byte _DIN[2048], _DOUT[42];

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void initDevice() {
  rgb = fset.deviceFlag & (1 << RGB_BIT);
  imu = fset.deviceFlag & (1 << IMU_BIT);
  irl = fset.deviceFlag & (1 << IRL_BIT);
  mot = fset.deviceFlag & (1 << MOT_BIT);

  if (rgb) initRGB();
  if (imu) initIMU();
  if (irl) initIRL();
  if (mot) initMOT();
}

void initSystem() {
  bli = gset.configFlag & (1 << BLI_BIT);
  bat = gset.configFlag & (1 << BAT_BIT);
  dgm = gset.configFlag & (1 << DGM_BIT);
  ben = gset.configFlag & (1 << BEN_BIT);
  osc = gset.configFlag & (1 << OSC_BIT);
  art = gset.configFlag & (1 << ART_BIT);
}

//-----------------------------------------------------------------------------------
// SETTINGS FUNCTIONS
//-----------------------------------------------------------------------------------

void setIdNumber() {
  gset.idNumber = _DIN[0] << 8 | _DIN[1];
}

void setDefaultIdNumber() {
  gset.idNumber = 0;
}

void setImuFlag() {
  gset.imuFlag = _DIN[0];
}

void setDefaultImuFlag() {
  gset.imuFlag = (1 << ACC_BIT);
}

void setConfigFlag() {
  gset.configFlag = _DIN[0] << 8 | _DIN[1];
}

void setDefaultConfigFlag() {
  gset.configFlag = (1 << DGM_BIT) | (1 << BAT_BIT) | (1 << BLI_BIT);
}
