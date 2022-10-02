//----------------------------------------------------------------------
// EEPROM SETTINGS V0.5
// OSC for Usine Creator
//
//  - eeprom adress change
//  - adding osc settings
//  - adding artnet settings
//----------------------------------------------------------------------

#if (MAJOR == 0) && (MINOR == 5) && (REVISION == 0)

//---------------------------------------------------------------
// CONFIG FLAG ADDRESSES (16 bits)
//---------------------------------------------------------------

#define BLI_BIT  0        // Blink leds at start
#define BAT_BIT  1        // Send automatic battery information
#define DGM_BIT  8        // Activate DGM protocol
#define BEN_BIT  9        // Activate BenTo protocol
#define ART_BIT 10        // Activate ArtNet protocol BETA
#define OSC_BIT 11        // Activate OSC protocol BETA
#define SPD_BIT 15        // Speed variation 50Hz (1) or 100Hz (0)

//---------------------------------------------------------------
// IMU FLAG ADDRESSES (8 bits)
//---------------------------------------------------------------

#define ACC_BIT  0        // Accelerometer
#define GYR_BIT  1        // Gyroscope
#define MAG_BIT  2        // Magnetometer
#define TMP_BIT  3        // Temperature
#define VEC_BIT  4        // IMU Magnitude
#define QUA_BIT  5        // Orientation Quaternion
#define WLD_BIT  6        // World Acceleration  
#define STA_BIT  7        // State Detection

//---------------------------------------------------------------
// EEPROM ADDRESSES
//---------------------------------------------------------------

#define ES_ADDR  0x00
#define FS_ADDR  0x10
#define GS_ADDR  0x20
#define DS_ADDR  0x30
#define BS_ADDR  0x50
#define OS_ADDR  0x70
#define AS_ADDR  0x90
#define WS_ADDR  0xB0

//---------------------------------------------------------------
// EEPROM DATA STRUCTURES (252 bytes)
//---------------------------------------------------------------

#define IP_LEN 16
#define PWD_LEN  32
#define SSID_LEN 32

// 0x00 Memory usage: 7/16 bytes
struct _eepromSettings {
  uint32_t credential;      // must be 0xFDB97531
  uint8_t  major;           // major version number
  uint8_t  minor;           // minor version number
  uint8_t  revision;        // revision version number
};

// 0x10 Memory usage: 8/16 bytes
struct _factorySettings {
  uint16_t serialNumber;    // unique identification number
  uint16_t deviceFlag;      // subsystems definition
  float adcCalibration;     // scale factor for adc
};

// 0x20 Memory usage: 7/16 bytes
struct _generalSettings {
  uint16_t idNumber;        // software identification number
  uint16_t imuFlag;         // IMU data flag
  uint16_t configFlag;      // Configuration flag
  bool     portalReboot;    // Reboot in portal mode
};

// 0x30 Memory usage: 8/32 bytes
struct _dgmSettings {
  uint32_t  outputIp;       // Client IP to send data to
  uint16_t  inputPort;      // Incoming port for datagram message
  uint16_t  outputPort;     // Outgoing port for datagram message
};

// 0x50 Memory usage: 10/32 bytes
struct _benSettings {
  uint32_t outputIp;        // Client IP to send data to
  uint16_t benInputPort;    // BenTo incoming port for datagram message
  uint16_t yoInputPort;     // Yo Protocol Input port
  uint16_t yoOutputPort;    // Yo Protocol Output port
};


// 0x70 Memory usage: 8/32 bytes
struct _oscSettings {
  uint32_t outputIp;        // Client IP to send data to
  uint16_t oscInputPort;    // OSC Protocol Input port
  uint16_t oscOutputPort;   // OSC Protocol Output port
};

// 0x90 Memory usage: 4/32 bytes
struct _artnetSettings {
  uint32_t universe;        // Artnet Universe
};

// 0xB0 Memory usage: 76 bytes
struct _wifiSettings {
  bool isStatic;
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
_oscSettings     oset;
_artnetSettings  aset;

void getParameters() {
  EEPROM.begin(512);
  EEPROM.get(ES_ADDR, eset);
  EEPROM.get(FS_ADDR, fset);
  EEPROM.get(GS_ADDR, gset);
  EEPROM.get(DS_ADDR, dset);
  EEPROM.get(BS_ADDR, bset);
  EEPROM.get(OS_ADDR, oset);
  EEPROM.get(AS_ADDR, aset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
}

void setDefaultParameters() {
  IPAddress tmpIp;

  eset.credential     = 0xECA8642;          // Eeprom magic number
  eset.major          = MAJOR;              // Eeprom major version
  eset.minor          = MINOR;              // Eeprom minor version
  eset.revision       = REVISION;           // Eeprom revision version

  fset.serialNumber   = SERIAL_NUMBER;      // Unique Serial Number
  fset.deviceFlag     = DEVICE_FLAG;        // 3 for les Objets Volants, 15 for v1 full, 31 for v2 full
  fset.adcCalibration = 1000.;              // ADC calibration

  gset.idNumber       = ID_NUMBER;          // Identification number
  gset.imuFlag        = 1 << ACC_BIT | 1 << GYR_BIT | 1 << MAG_BIT;
  gset.configFlag     = 1 << BLI_BIT | 1 << BAT_BIT | 1 << OSC_BIT |1 << SPD_BIT;
  gset.portalReboot   = false;

  tmpIp.fromString(DEFAULT_IP);
  dset.outputIp       = (uint32_t)tmpIp;    // default output IP
  dset.inputPort      = 8000;               // default input port
  dset.outputPort     = 9000;               // default output port

  tmpIp.fromString(DEFAULT_IP);
  bset.outputIp       = (uint32_t)tmpIp;    // default output IP
  bset.benInputPort   = 8888;               // default BenTo input port
  bset.yoInputPort    = 9000;               // default Yo protocol input port
  bset.yoOutputPort   = 10000;              // default Yo protocol output port

  tmpIp.fromString(DEFAULT_IP);
  oset.outputIp       = (uint32_t)tmpIp;    // default output IP
  oset.oscInputPort   = 10000;              // default OSC input port
  oset.oscOutputPort  = 12000;              // defautl OSC output port

  aset.universe       = 1;                  // default artnet universe

  strcpy(wset.ssid, DEFAULT_SSID);          // default ssid
  strcpy(wset.password, DEFAULT_PWD);       // default password
  tmpIp.fromString(DEFAULT_STATIC);
  wset.staticIp       = (uint32_t)tmpIp;    // default device IP
  tmpIp.fromString(DEFAULT_GATEWAY);
  wset.gateway        = (uint32_t)tmpIp;    // default gateway IP
  tmpIp.fromString(DEFAULT_MASK);
  wset.subnet         = (uint32_t)tmpIp;    // default subnetwork mask
  wset.isStatic       = false;
}

void saveDefaultParameters() {
  EEPROM.begin(512);
  EEPROM.put(ES_ADDR, eset);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.put(DS_ADDR, dset);
  EEPROM.put(BS_ADDR, bset);
  EEPROM.put(OS_ADDR, oset);
  EEPROM.put(AS_ADDR, aset);
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
  Serial.print("PR:\t");Serial.println(gset.portalReboot ? "true" : "false");;
  
  Serial.println("\n[ DGM Settings ]");
  Serial.print("IP: ");Serial.println(IPAddress(dset.outputIp));
  Serial.println("IN: " + String(dset.inputPort));
  Serial.println("OU: " + String(dset.outputPort));
  
  Serial.println("\n[ BenTo Settings ]");
  Serial.print("IP: ");Serial.println(IPAddress(bset.outputIp));
  Serial.println("IN: " + String(bset.benInputPort));
  Serial.println("YI: " + String(bset.yoInputPort));
  Serial.println("YO: " + String(bset.yoOutputPort));
  
  Serial.println("\n[ OSC Settings ]");
  Serial.print("IP: ");Serial.println(IPAddress(oset.outputIp));
  Serial.println("OI: " + String(oset.oscInputPort));
  Serial.println("OO: " + String(oset.oscOutputPort));
  
  Serial.println("\n[ Artnet Settings ]");
  Serial.println("UN: " + String(aset.universe));

  Serial.println("\n[ WiFi Settings ]");
  Serial.println("SSID:\t" + String(wset.ssid));
  Serial.println("PWD:\t" + String(wset.password));
  Serial.print("IP:\t");Serial.println(IPAddress(wset.staticIp));
  Serial.print("GW:\t");Serial.println(IPAddress(wset.gateway));
  Serial.print("SM:\t");Serial.println(IPAddress(wset.subnet));
  Serial.print("ST:\t");Serial.println(wset.isStatic ? "true" : "false");
}
#endif
