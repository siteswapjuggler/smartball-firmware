//-----------------------------------------------------------------------------------
// UDP SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP DGM;                       // datagram UDP socket
#define DGM_IN   8000              // datagram UDP in port
#define DGM_OUT  9000              // datagram UDP out port

IPAddress outIp(239, 0, 0, 51);    // multicast address of client applications
IPAddress multiIp(239, 0, 0, 50);  // multicast address of smartballs

boolean connectDGM() {
  return DGM.beginMulticast(WiFi.localIP(), multiIp, DGM_IN) == 1;
}

//-----------------------------------------------------------------------------------
// PACKET MANAGEMENT
//-----------------------------------------------------------------------------------

void receiveDGM() {
  uint16_t packetSize = DGM.parsePacket();
  if (packetSize)  {
    if (DGM.read() == SOP) {
      byte cmd = DGM.read();
      uint16_t len = DGM.read() << 8 | DGM.read();
      if (len) DGM.read(_DIN, len);
      if (DGM.read() == checkSum(cmd, len, _DIN)) {
        switch (cmd) {
          case CMD_PING:      sendPong();              break;
          case CMD_REBOOT:    espReboot();             break;
          case CMD_SLEEP:     espDeepSleep();          break;
          case CMD_FACTORY:   setFactorySettings();    break;
          case SAVE_FACTORY:  saveFactorySettings();   break;
          case CMD_COLOR1:    setRGB(0, len / 3, 0);   break;
          case CMD_COLOR2:    setRGB(1, len / 3, 0);   break;
          case CMD_STREAM:    setSTM((len - 1) / 3);   break;
          case CMD_IMU:       setIMU();                break;
          case SAVE_IMU:      saveImuSettings();       break;
          case CMD_ACCRANGE:  setAccRange();           break;
          case CMD_GYRRANGE:  setGyrRange();           break;
          case CMD_IRL:       setIRL();                break;
          case CMD_MOT:       setMOT();                break;
          case CMD_STB:       setSTB();                break;
          case CMD_MST:       setMST();                break;
        }
        printDebug(cmd, len, _DIN);
      }
      else printError("checksum");
    }
    else printError("packet");
  }
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void sendPong() {
  sendDgmAnswer(CMD_PING, 0);
}

void sendDgmAnswer(byte cmd, uint16_t len) {
  #ifdef UNIQUE_OUT_PORT
  uint16_t dgmOutPort = DGM_OUT + fset.serialNumber;
  #else 
  uint16_t dgmOutPort = DGM_OUT;
  #endif
  
  DGM.beginPacket(outIp, dgmOutPort);
  DGM.write(SOP);
  DGM.write(fset.serialNumber >> 8);
  DGM.write(fset.serialNumber & 255);
  DGM.write(cmd);
  DGM.write(len >> 8);
  DGM.write(len & 255);
  if (len) DGM.write(_DOUT, len);
  DGM.write(checkSum(cmd, len, _DOUT));
  DGM.endPacket();
}

uint8_t checkSum(uint8_t c, uint16_t l, uint8_t *data) {
  uint8_t sum = c + (l >> 8) + (l & 255);
  for (uint16_t i = 0; i < l; i++) sum += data[i];
  return sum;
}
