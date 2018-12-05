//-----------------------------------------------------------------------------------
// UDP SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP DGM;                       // datagram UDP socket
#define DGM_IN   8000              // datagram UDP in port
#define DGM_OUT  9000              // datagram UDP out port

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
    byte cmd = DGM.read();
    uint16_t len = packetSize - 1;
    if (len) DGM.read(_DIN, len);
    switch (cmd) {
      //------------------------------------------------
      // Those commands are low priority commands
      //------------------------------------------------

      case CMD_PING:      sendPong();              break;
      case CMD_REBOOT:    espReboot();             break;
      case CMD_SLEEP:     espDeepSleep();          break;
      case CMD_FACTORY:   setFactorySettings();    break;
      case SAVE_FACTORY:  saveFactorySettings();   break;
      case CMD_GENERAL:   setGeneralSettings();    break;
      case SAVE_GENERAL:  saveGeneralSettings();   break;
      case CMD_IMU:       setIMU();                break;
      case SAVE_IMU:      saveImuSettings();       break;
      case CMD_ACCRANGE:  setAccRange();           break;
      case CMD_GYRRANGE:  setGyrRange();           break;

      //------------------------------------------------
      // Those commands needs high bandwidth efficiency
      //------------------------------------------------
      
      case CMD_COLOR1:    setRGB(0, len / 3, 0);   break;
      case CMD_COLOR2:    setRGB(1, len / 3, 0);   break;
      case CMD_STREAM:    setSTM(len);             break;
      case CMD_IRL:       setIRL(0);               break;
      case CMD_MOT:       setMOT(0);               break;
      case CMD_STB:       setSTB(0);               break;
      case CMD_MST:       setMST(0);               break;
    }
    printDebug(cmd, len, _DIN);
  }
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void sendPong() {
  sendDgmAnswer(CMD_PING, 0);
}

void sendDgmAnswer(byte cmd, uint16_t len) {
  DGM.beginPacket(wset.outputIp, DGM_OUT);
  DGM.write(gset.idNumber >> 8);
  DGM.write(gset.idNumber & 255);
  DGM.write(fset.serialNumber >> 8);
  DGM.write(fset.serialNumber & 255);
  DGM.write(cmd);
  if (len) DGM.write(_DOUT, len);
  DGM.endPacket();
}

uint8_t checkSum(uint8_t c, uint16_t l, uint8_t *data) {
  uint8_t sum = c + (l >> 8) + (l & 255);
  for (uint16_t i = 0; i < l; i++) sum += data[i];
  return sum;
}
