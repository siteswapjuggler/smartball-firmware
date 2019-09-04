//-----------------------------------------------------------------------------------
// GLOBAL BUFFERS
//-----------------------------------------------------------------------------------

static byte _DIN[1024], _DOUT[42];

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

_factorySettings fset;
_imuSettings     iset;
_generalSettings gset;
_wifiSettings    wset;

//-----------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------------------------------------

void initEEPROM() {
  EEPROM.begin(512);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(IS_ADDR, iset);
  EEPROM.get(GS_ADDR, gset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
  printEepromDebug();
}

void espReboot() {
  ESP.restart();
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void setFactorySettings() {
  fset.serialNumber   = _DIN[0] << 8 | _DIN[1];
  fset.deviceFlag     = _DIN[2] << 8 | _DIN[3];
  fset.adcCalibration = (float)(_DIN[4] << 8 | _DIN[5]) / 10.;
}

void setGeneralSettings() {
  gset.idNumber = _DIN[0] << 8 | _DIN[1];
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveFactorySettings() {
  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.end();
}

void saveGeneralSettings() {
  EEPROM.begin(512);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.end();
}
