//---------------------------------------------------------------
// GLOBAL BUFFERS
//---------------------------------------------------------------

static byte _DIN[1024], _DOUT[42];

//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

_factorySettings factorySettings;
_imuSettings     imuSettings;

//---------------------------------------------------------------
// GENERAL FUNCTIONS
//---------------------------------------------------------------

void initEEPROM() {
  EEPROM.begin(512);
  EEPROM.get(FACTORY_SETTINGS_ADDR, factorySettings);
  EEPROM.get(IMU_SETTINGS_ADDR,     imuSettings);
  EEPROM.end();
}

void espReboot() {
  ESP.restart();
}

void espDeepSleep() {
  ESP.deepSleep(0);
}

//---------------------------------------------------------------
// DATAGRAM FUNCTIONS
//---------------------------------------------------------------

void setFactorySettings() {
  factorySettings.serialNumber   = _DIN[0]<<8 | _DIN[1];
  factorySettings.deviceFlag     = _DIN[2]<<8 | _DIN[3];
  factorySettings.adcCalibration = (float)(_DIN[4]<<8 | _DIN[5])/10.;
}

void saveFactorySettings() {
  EEPROM.begin(512);
  EEPROM.put(FACTORY_SETTINGS_ADDR, factorySettings);
  EEPROM.end();
}
