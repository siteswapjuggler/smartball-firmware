//-----------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//-----------------------------------------------------------------------------------

unsigned long debugTime;

void initDebug() {
#if defined(DGM_DEBUG) || defined(TIME_DEBUG) || defined(EEPROM_DEBUG)
  Serial.begin(1000000);
#endif
}

void getDebugTime() {
#ifdef TIME_DEBUG
  debugTime = micros();
#endif
}

void printTimeDebug(const char* msg) {
#ifdef TIME_DEBUG
  Serial.print(msg);
  Serial.println(micros() - debugTime);
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
  Serial.println("--------- Factory Settings ---------");
  Serial.println();
  Serial.print("Serial number: ");
  Serial.println(fset.serialNumber);
  Serial.print("Device flag: ");
  Serial.println(fset.deviceFlag);
  Serial.print("ADC calibration: ");
  Serial.println(fset.adcCalibration);
  Serial.println();
  Serial.println("--------- IMU Settings ---------");
  Serial.println();
  Serial.print("Stream Flag: ");
  Serial.println(iset.streamFlag);
  Serial.print("Accelerometer Range: ");
  Serial.println(iset.accRange);
  Serial.print("Gyroscope Range: ");
  Serial.println(iset.gyrRange);
  Serial.println();
  Serial.println("--------- WiFi Settings ---------");
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(wset.ssid);
  Serial.print("Password: ");
  Serial.println(wset.password);
  Serial.print("Output IP: ");
  Serial.println(wset.outputIp);
  Serial.println();
  Serial.println("--------- General Settings ---------");
  Serial.println();
  Serial.print("ID number: ");
  Serial.println(gset.idNumber);
  Serial.println();
#endif
}
