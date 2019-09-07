//-----------------------------------------------------------------------------------
// GLOBAL BUFFERS
//-----------------------------------------------------------------------------------

static byte _DIN[1024], _DOUT[42];

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
}

//-----------------------------------------------------------------------------------
// SETTINGS FUNCTIONS
//-----------------------------------------------------------------------------------

void setIdNumber() {
  gset.idNumber = _DIN[0] << 8 | _DIN[1];
}

void setDefautlIdNumber() {
  gset.idNumber = 0;
}

void setConfigFlag() {
  gset.configFlag = _DIN[0] << 8 | _DIN[1];
}

void setDefaultConfigFlag(){
  gset.configFlag = (1 << DGM_BIT) | (1 << BAT_BIT) | (1 << BLI_BIT);
}

void setImuFlag() {
  gset.imuFlag = _DIN[0] << 8 | _DIN[1];
}

void setDefaultImuFlag() {
  gset.imuFlag = (1 << VEC_BIT) | (1 << STA_BIT);
}

void setDefautlFactorySettings() {
  fset.serialNumber = 43;
  fset.deviceFlag = 15;
  fset.adcCalibration = 1000.;
}

void setDefaultEepromSetting() {
  eset.credential = 0xFDB97531;
  eset.major = 0;
  eset.minor = 4;
  eset.revision = 0;
}

void setDefaultWifiSettings() {
  strcpy(wset.ssid, "ssid");
  strcpy(wset.password, "password");
}

void setFactorySettings() {
  fset.serialNumber   = _DIN[0] << 8 | _DIN[1];
  fset.deviceFlag     = _DIN[2] << 8 | _DIN[3];
  fset.adcCalibration = (float)(_DIN[4] << 8 | _DIN[5]) / 10.;
}

void resetSettings() {
  setDefautlIdNumber();
  setDefaultConfigFlag();
  setDefaultImuFlag();
  setDefaultEepromSetting();
  setDefaultWifiSettings();
  setDefaultBenSettings();
  setDefaultDgmSettings();
  setDefautlFactorySettings();

  saveEepromSettings();
  saveFactorySettings();
  saveGeneralSettings();
  saveDgmSettings();
  saveBenSettings();
  saveWifiSettings();
}
