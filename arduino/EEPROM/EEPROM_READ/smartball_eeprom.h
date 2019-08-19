#ifndef VERSION
#define VERSION 0
#endif

//----------------------------------------------------------------------
// EEPROM SETTINGS V0
// first firmware for "les Objets Volants"
//----------------------------------------------------------------------

#if (EEPROM_VERSION==0)

#define FACTORY_SETTINGS_ADDR   0x00
#define IMU_SETTINGS_ADDR       0x10

struct _factorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte accSmoothing;
  byte gyrRange;
  byte gyrSmoothing;
  byte magSmoothing;
};

_factorySettings fset;
_imuSettings     iset;

//----------------------------------------------------------------------
// EEPROM SETTINGS V1
// second firmware for "les Objets Volants"
//
//  - removing Smoothing parameters
//  - adding WiFi settings
//
//----------------------------------------------------------------------

#elif (EEPROM_VERSION==1)

#define FS_ADDR  0x00
#define IS_ADDR  0x10
#define WS_ADDR  0x40

struct _factorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte gyrRange;
};

struct _wifiSettings {
  char ssid[64];
  char password[64];
};

_factorySettings fset;
_imuSettings     iset;
_wifiSettings    wset;

//----------------------------------------------------------------------
// EEPROM SETTINGS V2
// third firmware for "les Objets Volants" - 2019
//
//  - adding General Settings
//  - adding Output IP in WiFi settings
//
//----------------------------------------------------------------------

#elif (EEPROM_VERSION==2)

#define FS_ADDR  0x00
#define IS_ADDR  0x10
#define GS_ADDR  0x20
#define WS_ADDR  0x40

#define IP_LEN 16
#define PWD_LEN  64
#define SSID_LEN 64

struct _factorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte gyrRange;
};

struct _generalSettings {
  uint16_t idNumber;
};

struct _wifiSettings {
  char ssid[SSID_LEN];
  char password[PWD_LEN];
  char outputIp[IP_LEN];
};

_factorySettings fset;
_imuSettings     iset;
_generalSettings gset;
_wifiSettings    wset;

#endif
