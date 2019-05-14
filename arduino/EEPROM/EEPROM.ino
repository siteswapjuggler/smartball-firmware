//-----------------------------------------------------------------------------------
// BOARD VERSION
//-----------------------------------------------------------------------------------

#define BOARD_VERSION 1        // 1 - Alpha and first batch for les Objets Volants
#include "header/smartball.h"  // Smartball specific definitions

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
}

//-----------------------------------------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------------------------------------

void factorySettings() {
  gset.idNumber = 0;

  fset.serialNumber   = 33;
  fset.deviceFlag     = 3; //fset.serialNumber ? 3 : 15;
  fset.adcCalibration = 1000.;

  iset.streamFlag = 0;
  iset.accRange   = 3;
  iset.gyrRange   = 3;
  
  strcpy(wset.ssid, "DOME_FORMATION");
  strcpy(wset.password, "dome_formation");
  strcpy(wset.outputIp, "192.168.200.10");
  
  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(IS_ADDR, iset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}

