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
  //getDebugTime(); // quand packet ça prend 13 µs sinon 1 µs quand boucle sans travail
  uint16_t packetSize = DGM.parsePacket();
  
  //getDebugTime(); // quand packet ça prend 13 µs sinon 1 µs quand boucle sans travail
  if (packetSize)  {
    byte cmd = DGM.read();
    uint16_t len = packetSize - 1;
    // ici verifier que len n'est supérieur à la capacité du buffer
    if (len) DGM.read(_DIN, len);
    switch (cmd) {
      //------------------------------------------------
      // Those commands are low priority commands
      //------------------------------------------------

      case CMD_PING:      sendPong();              break;
      case CMD_REBOOT:    espReboot();             break;

      case CMD_GENERAL:   setIdNumber();           break;
      case SAVE_GENERAL:  saveGeneralSettings();   break;

      case CMD_IMU:       setImuFlag();            break;
      case DEFAULT_IMU:   setDefaultImuFlag();     break;

      //------------------------------------------------
      // Those commands needs high bandwidth efficiency
      //------------------------------------------------

      case CMD_COLOR1:    setRGB(0, len / 3, 0);   break;
      case CMD_COLOR2:    setRGB(1, len / 3, 0);   break;
      case CMD_WHITE1:    setW(0, len, 0);         break;
      case CMD_WHITE2:    setW(1, len, 0);         break;
      case CMD_STREAM:    setSTM(len);             break;
      case CMD_IRL:       setIRL(0);               break;
      case CMD_MOT:       setMOT(0);               break;
      case CMD_STB:       setSTB(0);               break;
      case CMD_MST:       setMST(0);               break;
    }
    printDgmDebug(cmd, len, _DIN);
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
