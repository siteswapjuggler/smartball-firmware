//-----------------------------------------------------------------------------------
// UDP SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP DGM;                       // datagram UDP socket
IPAddress multiIp(239, 0, 0, 50);  // multicast address of smartballs

boolean connectDGM() {
  return DGM.beginMulticast(WiFi.localIP(), multiIp, dset.inputPort) == 1;
}

//-----------------------------------------------------------------------------------
// PACKET MANAGEMENT
//-----------------------------------------------------------------------------------

void receiveDGM() {
  uint16_t packetSize = DGM.parsePacket();
  if (packetSize)  {
    printTimeDebug("packet: ");
    getDebugTime();
    byte cmd = DGM.read();
    uint16_t len = packetSize - 1;
    if (len) DGM.read(_DIN, len);
    switch (cmd) {
      //------------------------------------------------
      // Those commands are low priority commands
      //------------------------------------------------

      case CMD_PING:      sendPong();              break;
      case CMD_REBOOT:    espReboot();             break;
      
      case CMD_FACTORY:   setFactorySettings();    break;
      case SAVE_FACTORY:  saveFactorySettings();   break;
      
      case CMD_GENERAL:   setIdNumber();           break;
      case SAVE_GENERAL:  saveGeneralSettings();   break;

      case CMD_IMU:       setImuFlag();            break;
      case DEFAULT_IMU:   setDefaultImuFlag();     break;

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
    printMsgDebug(cmd, len, _DIN);
  }
}

//-----------------------------------------------------------------------------------
// DATAGRAM FUNCTIONS
//-----------------------------------------------------------------------------------

void sendPong() {
  sendDgmAnswer(CMD_PING, 0);
}

void sendDgmAnswer(byte cmd, uint16_t len) {
  DGM.beginPacket(dset.outputIp, dset.outputPort);
  DGM.write(gset.idNumber >> 8);
  DGM.write(gset.idNumber & 255);
  DGM.write(fset.serialNumber >> 8);
  DGM.write(fset.serialNumber & 255);
  DGM.write(cmd);
  if (len) DGM.write(_DOUT, len);
  DGM.endPacket();
}

void setDefaultDgmSettings() {
  strcpy(dset.outputIp,"239.0.0.51");
  dset.inputPort  = 8000;
  dset.outputPort = 9000;
}
