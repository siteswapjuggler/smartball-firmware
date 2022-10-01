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

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

#define MAJOR    0
#define MINOR    4
#define REVISION 0

#define SERIAL_NUMBER   30
#define ID_NUMBER       SERIAL_NUMBER
#define DEVICE_FLAG     0b11111
#define DEFAULT_SSID    "SmartBall1420"
#define DEFAULT_PWD     "5m@rtB@ll1420"
#define DEFAULT_IP      "192.168.100.25"
#define DEFAULT_STATIC  "192.168.100.130"
#define DEFAULT_GATEWAY "192.168.100.1"
#define DEFAULT_MASK    "255.255.255.0"

void setDefaultParameters() {
  IPAddress tmpIp;

  eset.credential     = 0xECA8642;          // Eeprom magic number (1420)
  eset.major          = MAJOR;              // Eeprom major version
  eset.minor          = MINOR;              // Eeprom minor version
  eset.revision       = REVISION;           // Eeprom revision version

  fset.serialNumber   = SERIAL_NUMBER;      // Unique Serial Number
  fset.deviceFlag     = DEVICE_FLAG;        // 3 for les Objets Volants, 15 for full devices
  fset.adcCalibration = 1000.;              // ADC calibration

  gset.idNumber       = ID_NUMBER;          // Identification number
//  gset.imuFlag        = 0b11111111;         // Only accelerometer
  gset.imuFlag        = 0;
//  gset.configFlag     = 0b1100000011;       // Blink, batt, bento & datagram
  gset.configFlag     = 0b0000010100000011; // Blink, batt, dgm, artnet

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
  Serial.println("OI: " + String(bset.oscInputPort));
  Serial.println("OO: " + String(bset.oscOutputPort));
  Serial.println("\n[ WiFi Settings ]");
  Serial.println("SSID:\t" + String(wset.ssid));
  Serial.println("PWD:\t" + String(wset.password));
  Serial.print("IP:\t");Serial.println(IPAddress(wset.staticIp));
  Serial.print("GW:\t");Serial.println(IPAddress(wset.gateway));
  Serial.print("SM:\t");Serial.println(IPAddress(wset.subnet));
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
