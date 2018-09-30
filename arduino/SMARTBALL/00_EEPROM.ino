//-----------------------------------------------------------------------------------
// GLOBAL BUFFERS
//-----------------------------------------------------------------------------------

static byte _DIN[1024], _DOUT[42];

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

_factorySettings fset;
_imuSettings     iset;
_wifiSettings    wset;

//-----------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------------------------------------

void initEEPROM() {
  EEPROM.begin(512);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(IS_ADDR, iset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
}

void espReboot() {
  ESP.restart();
}

void espDeepSleep() {
  ESP.deepSleep(0);
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void setFactorySettings() {
  fset.serialNumber   = _DIN[0] << 8 | _DIN[1];
  fset.deviceFlag     = _DIN[2] << 8 | _DIN[3];
  fset.adcCalibration = (float)(_DIN[4] << 8 | _DIN[5]) / 10.;
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveFactorySettings() {
  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.end();
}
