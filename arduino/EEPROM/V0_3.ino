//----------------------------------------------------------------------
// EEPROM SETTINGS V0.3
// third firmware for "les Objets Volants"
//
//  - adding General Settings
//  - adding Output IP in WiFi settings
//----------------------------------------------------------------------

#if (MAJOR == 0) && (MINOR == 3) && (REVISION == 0)

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

void getParameters() {
  EEPROM.begin(512);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(IS_ADDR, iset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.get(GS_ADDR, gset);
  EEPROM.end();
}

void setDefaultParameters() {
  fset.serialNumber   = SERIAL_NUMBER;      // Unique Serial Number
  fset.deviceFlag     = DEVICE_FLAG;        // 3 for les Objets Volants, 15 for full devices
  fset.adcCalibration = 1000.;              // ADC calibration
  gset.idNumber       = 0;                  // Identification number
  iset.streamFlag     = 1;                  // Only Acceleration
  iset.accRange       = 3;                  // Maximum range +- 16G
  iset.gyrRange       = 3;                  // Maximum range +- 2000DPS
  strcpy(wset.ssid, DEFAULT_SSID);          // default ssid
  strcpy(wset.password, DEFAULT_PWD);       // default password
  strcpy(wset.outputIp, DEFAULT_IP);        // default output IP
}

void saveDefaultParameters() {
  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(IS_ADDR, iset);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.end();
}

void printParameters() {
  Serial.println("\n[ Factory Settings ]");
  Serial.println("SN: " + String(fset.serialNumber));
  Serial.println("DF: 0b" + String(fset.deviceFlag, BIN));
  Serial.println("AC: " + String(fset.adcCalibration, 2));
  Serial.println("\n[ IMU Settings ]");
  Serial.println("SF: 0b" + String(iset.streamFlag, BIN));
  Serial.println("AR: " + String(iset.accRange));
  Serial.println("GR: " + String(iset.gyrRange));
  Serial.println("\n[ General Settings ]");
  Serial.println("ID: " + String(gset.idNumber));
  Serial.println("\n[ WiFi Settings ]");
  Serial.println("SSID:\t" + String(wset.ssid));
  Serial.println("PWD:\t" + String(wset.password));
  Serial.println("IP:\t" + String(wset.outputIp));
}
#endif
