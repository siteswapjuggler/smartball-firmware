void printParameters() {
#if (EEPROM_VERSION==0)
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
  Serial.print("Accelerometer Smoothing: ");
  Serial.println(iset.accSmoothing);
  Serial.print("Gyroscope Range: ");
  Serial.println(iset.gyrRange);
  Serial.print("Gyroscope Smoothing: ");
  Serial.println(iset.gyrSmoothing);
  Serial.print("Magnetometer Smoothing: ");
  Serial.println(iset.magSmoothing);
  Serial.println();
#elif (EEPROM_VERSION==1)
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
  Serial.println();
#elif (EEPROM_VERSION==2)
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
