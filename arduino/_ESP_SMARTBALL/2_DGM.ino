//---------------------------------------------------------------
// MESSAGES MANAGEMENT
//---------------------------------------------------------------

void receiveDatagram() {
  if (UDP.parsePacket())  {
    while (UDP.available()) {
      if (UDP.read() == SOP) {
        byte cmd = UDP.read();
        uint16_t len = UDP.read() << 8 | UDP.read();
        if (len) UDP.read(_DIN, len);
        if (UDP.read() == checkSum(cmd, len, _DIN)) {
          switch (cmd) {
            case CMD_PING:      sendPong();            break;
            case CMD_REBOOT:    espReboot();           break;
            case CMD_SLEEP:     espDeepSleep();        break;
            case CMD_FACTORY:   setFactorySettings();  break;
            case SAVE_FACTORY:  saveFactorySettings(); break;
            case CMD_COLOR:     setRGB(len / 3, 0);    break;
            case CMD_STREAM:    setSTM((len - 1) / 3); break;
            case CMD_IMU:       setIMU();              break;
            case SAVE_IMU:      saveImuSettings();     break;
            case CMD_ACCRANGE:  setAccRange();         break;
            case CMD_GYRRANGE:  setGyrRange();         break;
            case CMD_IRL:       setIRL();              break;
            case CMD_MOT:       setMOT();              break;
          }
          printDebug(cmd, len, _DIN);
        }
        else printError("checksum");
      }
      else printError("packet");
    }
  }
}

//---------------------------------------------------------------
// PROTOCOL FUNCTIONS
//---------------------------------------------------------------

void sendPong() {
  sendDgmAnswer(CMD_PING, 0);
}

void sendDgmAnswer(byte cmd, uint16_t len) {
  UDP.beginPacket(outIp, dgmOutPort);
  UDP.write(SOP);
  UDP.write(factorySettings.serialNumber >> 8);
  UDP.write(factorySettings.serialNumber & 255);
  UDP.write(cmd);
  UDP.write(len >> 8);
  UDP.write(len & 255);
  if (len) UDP.write(_DOUT, len);
  UDP.write(checkSum(cmd, len, _DOUT));
  UDP.endPacket();
}

uint8_t checkSum(uint8_t c, uint16_t l, uint8_t *data) {
  uint8_t sum = c + (l >> 8) + (l & 255);
  for (uint16_t i = 0; i < l; i++) sum += data[i];
  return sum;
}

//---------------------------------------------------------------
// DEBUG FUNCTIONS
//---------------------------------------------------------------

void initDebug() {
#ifdef DEBUG
  Serial.begin(1000000);
  previousTime = 0;
#endif
}

void printDebug(uint8_t c, uint16_t l, uint8_t *data) {
#ifdef DEBUG
  Serial.print("CMD: " + String(c,HEX) + "\t LEN: " + String(l) + "\t DATA: ");
  for (int i = 0; i < l; i++) {
    Serial.print(String(data[i]));
    if (i + 1 < l ) Serial.print(" ");
  }
  Serial.println();

  long actualTime = millis();
  Serial.println("prev dgm: "+String(actualTime - previousTime));
  prieviousTime = actualTime;
#endif
}

void printError(const char *error) {
#ifdef DEBUG
  Serial.print("ERROR: ");
  Serial.println(error);
#endif
}
