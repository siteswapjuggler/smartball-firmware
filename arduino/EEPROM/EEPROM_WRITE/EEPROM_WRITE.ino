//-----------------------------------------------------------------------------------
// BOARD VERSION
//-----------------------------------------------------------------------------------

#define BOARD_VERSION 1        // 1 - Alpha and first batch for les Objets Volants
#include "SMARTBALL_DEF.h"     // Smartball specific definitions

//-----------------------------------------------------------------------------------
// LIBRARIES
//-----------------------------------------------------------------------------------

#include <EEPROM.h>            // 512 byte EEPROM Emulation for ESP8266

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

_factorySettings fset;
_imuSettings     iset;
_generalSettings gset;
_wifiSettings    wset;

//-----------------------------------------------------------------------------------
// INITIAL SETUP
//-----------------------------------------------------------------------------------

void setup() {
  factorySettings();
}

//-----------------------------------------------------------------------------------
// NO LOOP
//-----------------------------------------------------------------------------------

void loop() {
  // prevent from crashing
  yield();
  delay(1);
}

//-----------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------------------------------------

void factorySettings() {
  gset.idNumber = 0;

  fset.serialNumber   = 43;     // depend on the ball your actually flashing
  fset.deviceFlag     = 15;     // 3 for les Objets Volants, 15 for full devices
  fset.adcCalibration = 1000.;

  iset.streamFlag = 0;
  iset.accRange   = 3;
  iset.gyrRange   = 3;
  
  strcpy(wset.ssid, "ssid");
  strcpy(wset.password, "password");
  strcpy(wset.outputIp, "239.0.0.51");
  
  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(IS_ADDR, iset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}
