void getParameters() {
  EEPROM.begin(512);
  #if (EEPROM_VERSION==0)
  EEPROM.get(FACTORY_SETTINGS_ADDR, fset);
  EEPROM.get(IMU_SETTINGS_ADDR, iset);
  #elif (EEPROM_VERSION==1)
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(IS_ADDR, iset);
  EEPROM.get(WS_ADDR, wset);
  #elif (EEPROM_VERSION==2)
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(IS_ADDR, iset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.get(GS_ADDR, gset);
  #endif
  EEPROM.end();
}
