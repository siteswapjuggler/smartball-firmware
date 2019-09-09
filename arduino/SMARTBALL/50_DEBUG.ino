//-----------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//-----------------------------------------------------------------------------------

unsigned long debugTime;

void initDebug() {
#if defined(DGM_DEBUG) || defined(TIME_DEBUG) || defined(EEPROM_DEBUG)
  Serial.begin(115200);
#endif
}

void getDebugTime() {
#ifdef TIME_DEBUG
  debugTime = micros();
#endif
}

void printTimeDebug(const char* msg) {
#ifdef TIME_DEBUG
  uint32_t delta = micros() - debugTime;
  Serial.print(msg);
  Serial.println(delta);
#endif
}

void printMsgDebug(uint8_t c, uint16_t l, uint8_t *data) {
#ifdef DGM_DEBUG
  Serial.print("CMD: " + String(c, HEX) + "\t LEN: " + String(l));
  if (l) {
    Serial.print("\t DATA: ");
    for (int i = 0; i < l; i++) {
      Serial.print(String(data[i]));
      if (i + 1 < l ) Serial.print(" ");
    }
  }
  Serial.println();
#endif
}

void printError(const char *error) {
#ifdef DGM_DEBUG
  Serial.print("ERROR: ");
  Serial.println(error);
#endif
}

void printEepromDebug() {
#ifdef EEPROM_DEBUG
  Serial.println("--------- Eeprom Settings ---------");
  Serial.println();
  Serial.print("Eeprom credential: ");
  Serial.println(eset.credential,HEX);
  Serial.print("Eeprom version: ");
  Serial.println(eepromVersion);
  Serial.println("--------- Factory Settings ---------");
  Serial.println();
  Serial.print("Serial number: ");
  Serial.println(fset.serialNumber);
  Serial.print("Device flag: ");
  Serial.println(fset.deviceFlag,BIN);
  Serial.print("ADC calibration: ");
  Serial.println(fset.adcCalibration);
  Serial.println();
  Serial.println("--------- General Settings ---------");
  Serial.println();
  Serial.print("ID number: ");
  Serial.println(gset.idNumber);
  Serial.print("Config Flag: ");
  Serial.println(gset.configFlag, BIN);
  Serial.print("IMU Flag: ");
  Serial.println(gset.imuFlag, BIN);
  Serial.println();
  Serial.println("--------- DGM Settings ---------");
  Serial.println();
  Serial.print("Output IP: ");
  Serial.println(dset.outputIp);
  Serial.print("Input Port: ");
  Serial.println(dset.inputPort);
  Serial.print("Output Port: ");
  Serial.println(dset.outputPort);
  Serial.println();
  Serial.println("--------- BenTo Settings ---------");
  Serial.println();
  Serial.print("Output IP: ");
  Serial.println(bset.outputIp);
  Serial.print("BenTo Input Port: ");
  Serial.println(bset.benInputPort);
  Serial.print("OSC Input Port: ");
  Serial.println(bset.oscInputPort);
  Serial.print("OSC Output Port: ");
  Serial.println(bset.oscOutputPort);
  Serial.println();
  Serial.println("--------- WiFi Settings ---------");
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(wset.ssid);
  Serial.print("Password: ");
  Serial.println(wset.password);
  Serial.println();
#endif
}
