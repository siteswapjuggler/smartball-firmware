//----------------------------------------------------------------------
// EEPROM SETTINGS V0.4
// fourth firmware for "les Objets Volants"
//
//  - adding eeprom versionning
//  - factory settings address change
//  - removing imu settings
//  - completer general settings (imuFlag moved here, configFlag added)
//  - adding dgm settings and bento settings
//  - wifi settings now 32 char long
//----------------------------------------------------------------------

#if (MAJOR == 0) && (MINOR == 4) && (REVISION == 0)

#define ES_ADDR  0x00
#define FS_ADDR  0x10
#define GS_ADDR  0x20
#define DS_ADDR  0x30
#define BS_ADDR  0x50
#define WS_ADDR  0x70

#define IP_LEN 16
#define PWD_LEN  32
#define SSID_LEN 32

struct _eepromSettings {
  uint32_t credential;      // must be 0xFDB97531
  uint8_t  major;           // major version number
  uint8_t  minor;           // minor version number
  uint8_t  revision;        // revision version number
};

struct _factorySettings {
  uint16_t serialNumber;    // unique ID depending on manufacturing
  uint16_t deviceFlag;      // subsystems definition
  float adcCalibration;     // scale factor for adc
};

struct _generalSettings {
  uint16_t idNumber;
  uint16_t imuFlag;
  uint16_t configFlag;
};

struct _dgmSettings {
  uint32_t  outputIp;
  uint16_t  inputPort;
  uint16_t  outputPort;
};

struct _benSettings {
  uint32_t outputIp;
  uint16_t benInputPort;
  uint16_t oscInputPort;
  uint16_t oscOutputPort;
};

struct _wifiSettings {
  char ssid[SSID_LEN];
  char password[PWD_LEN];
  uint32_t staticIp;
  uint32_t gateway;
  uint32_t subnet;
};

_eepromSettings  eset;
_factorySettings fset;
_generalSettings gset;
_wifiSettings    wset;
_dgmSettings     dset;
_benSettings     bset;

void getParameters() {
  EEPROM.begin(512);
  EEPROM.get(ES_ADDR, eset);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(GS_ADDR, gset);
  EEPROM.get(DS_ADDR, dset);
  EEPROM.get(BS_ADDR, bset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
}

void setDefaultParameters() {
  IPAddress tmpIp;

  eset.credential     = 0xFDB97531;         // Eeprom magic number
  eset.major          = 0;                  // Eeprom major version
  eset.minor          = 4;                  // Eeprom minor version
  eset.revision       = 0;                  // Eeprom revision version

  fset.serialNumber   = SERIAL_NUMBER;      // Unique Serial Number
  fset.deviceFlag     = DEVICE_FLAG;        // 3 for les Objets Volants, 15 for full devices
  fset.adcCalibration = 1000.;              // ADC calibration

  gset.idNumber       = ID_NUMBER;          // Identification number
  gset.imuFlag        = 1;                  // Only accelerometer
  gset.configFlag     = 0b100000011;        // Blink, batt & datagram

  tmpIp.fromString(DEFAULT_IP);
  dset.outputIp       = (uint32_t)tmpIp;    // default output IP
  dset.inputPort      = 8000;               // default input port
  dset.outputPort     = 9000;               // default output port

  tmpIp.fromString(DEFAULT_IP);
  bset.outputIp       = (uint32_t)tmpIp;    // default output IP
  bset.benInputPort   = 8888;               // default BenTo input port
  bset.oscInputPort   = 9000;               // default Yo protocol input port
  bset.oscOutputPort  = 10000;              // default Yo protocol output port
  
  strcpy(wset.ssid, DEFAULT_SSID);          // default ssid
  strcpy(wset.password, DEFAULT_PWD);       // default password
  tmpIp.fromString(DEFAULT_STATIC);
  wset.staticIp       = (uint32_t)tmpIp;    // default device IP
  tmpIp.fromString(DEFAULT_GATEWAY);
  wset.gateway        = (uint32_t)tmpIp;    // default gateway IP
  tmpIp.fromString(DEFAULT_MASK);
  wset.subnet         = (uint32_t)tmpIp;    // default subnetwork mask
}

void saveDefaultParameters() {
  EEPROM.begin(512);
  EEPROM.put(ES_ADDR, eset);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.put(DS_ADDR, dset);
  EEPROM.put(BS_ADDR, bset);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}

void printParameters() {
  Serial.println("\n[ Eeprom Settings ]");
  Serial.println("EC: 0x" + String(eset.credential, HEX));
  Serial.println("EV: " + eepromVersion);
  Serial.println("\n[ Factory Settings ]");
  Serial.println("SN: " + String(fset.serialNumber));
  Serial.println("DF: 0b" + String(fset.deviceFlag, BIN));
  Serial.println("AC: " + String(fset.adcCalibration, 2));
  Serial.println("\n[ General Settings ]");
  Serial.println("ID: " + String(gset.idNumber));
  Serial.println("IF: 0b" + String(gset.imuFlag, BIN));
  Serial.println("CF: 0b" + String(gset.configFlag, BIN));
  Serial.println("\n[ DGM Settings ]");
  Serial.print("IP: ");Serial.println(IPAddress(dset.outputIp));
  Serial.println("IN: " + String(dset.inputPort));
  Serial.println("OU: " + String(dset.outputPort));
  Serial.println("\n[ BenTo Settings ]");
  Serial.print("IP: ");Serial.println(IPAddress(bset.outputIp));
  Serial.println("IN: " + String(bset.benInputPort));
  Serial.println("OI: " + String(bset.oscInputPort));
  Serial.println("OO: " + String(bset.oscOutputPort));
  Serial.println("\n[ WiFi Settings ]");
  Serial.println("SSID:\t" + String(wset.ssid));
  Serial.println("PWD:\t" + String(wset.password));
  Serial.print("IP:\t");Serial.println(IPAddress(wset.staticIp));
  Serial.print("GW:\t");Serial.println(IPAddress(wset.gateway));
  Serial.print("SM:\t");Serial.println(IPAddress(wset.subnet));
}
#endif
