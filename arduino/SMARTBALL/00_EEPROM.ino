//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

_eepromSettings  eset;
_factorySettings fset;
_generalSettings gset;
_wifiSettings    wset;
_dgmSettings     dset;
_benSettings     bset;
_oscSettings     oset;
_artnetSettings  aset;

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
  EEPROM.get(OS_ADDR, oset);
  EEPROM.get(AS_ADDR, aset);
  EEPROM.get(WS_ADDR, wset);
  EEPROM.end();
  eepromVersion = getEepromVersion();

  if (eset.credential != 0xECA8642) {
    setDefaultParameters();
    saveDefaultParameters();
    espReboot();
  }
}

String getEepromVersion() {
  String v = String(eset.major)+"."+String(eset.minor)+"."+String(eset.revision);
  return v;
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

bool isPortalReboot() {
  return gset.portalReboot;
}

void setPortalReboot(bool b) {
  gset.portalReboot = b;
  saveGeneralSettings();
}

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

void saveOSCSettings() {
  EEPROM.begin(512);
  EEPROM.put(OS_ADDR, oset);
  EEPROM.end();
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

#define MAJOR    0
#define MINOR    5
#define REVISION 0

#define SERIAL_NUMBER   37
#define ID_NUMBER       SERIAL_NUMBER
#define DEVICE_FLAG     0b1111
#define DEFAULT_SSID    "network"
#define DEFAULT_PWD     "password"
#define DEFAULT_IP      "192.168.1.10"
#define DEFAULT_STATIC  "192.168.1.100"
#define DEFAULT_GATEWAY "192.168.1.1"
#define DEFAULT_MASK    "255.255.255.0"

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

void printEepromParameters() {
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
