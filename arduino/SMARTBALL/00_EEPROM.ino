//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

_eepromSettings  eset;
_factorySettings fset;
_generalSettings gset;
_wifiSettings    wset;
_dgmSettings     dset;
_benSettings     bset;

//-----------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------------------------------------

void initEeprom() {
  EEPROM.begin(512);
  EEPROM.get(ES_ADDR, eset);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(GS_ADDR, gset);
  EEPROM.get(DS_ADDR, dset);
  EEPROM.get(BS_ADDR, bset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
  eepromVersion = getEepromVersion();
}

String getEepromVersion() {
  String v = String(eset.major)+"."+String(eset.minor)+"."+String(eset.revision);
  return v;
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveGeneralSettings() {
  EEPROM.begin(512);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.end();
}

void saveWifiSettings() {
  EEPROM.begin(512);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}
