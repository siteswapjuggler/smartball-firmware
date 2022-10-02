//----------------------------------------------------------------------
// EEPROM SETTINGS V0.1
// first firmware for "les Objets Volants"
//----------------------------------------------------------------------

#if (MAJOR == 0) && (MINOR == 1) && (REVISION == 0)

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

void getParameters() {
  EEPROM.begin(512);
  EEPROM.get(FACTORY_SETTINGS_ADDR, fset);
  EEPROM.get(IMU_SETTINGS_ADDR, iset);
  EEPROM.end();
}

void setDefaultParameters() {
  fset.serialNumber   = SERIAL_NUMBER;      // Unique Serial Number
  fset.deviceFlag     = DEVICE_FLAG;        // 3 for les Objets Volants, 15 for v1 full, 31 for v2 full
  fset.adcCalibration = 1000.;              // ADC calibration
  iset.streamFlag     = 1;                  // Only Acceleration
  iset.accRange       = 3;                  // Maximum range +- 16G
  iset.accSmoothing   = 0;                  // not used
  iset.gyrRange       = 3;                  // Maximum range +- 2000DPS
  iset.gyrSmoothing   = 0;                  // not used
  iset.magSmoothing   = 0;                  // not used
}

void saveDefaultParameters() {
  EEPROM.begin(512);
  EEPROM.put(FACTORY_SETTINGS_ADDR, fset);
  EEPROM.put(IMU_SETTINGS_ADDR, iset);
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
  Serial.println("AS: " + String(iset.accSmoothing));
  Serial.println("GR: " + String(iset.gyrRange));
  Serial.println("GS: " + String(iset.gyrSmoothing));
  Serial.println("MS: " + String(iset.magSmoothing));
}
#endif
