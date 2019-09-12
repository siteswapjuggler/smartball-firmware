//-----------------------------------------------------------------------------------
// BENTO PROTOCOL
//-----------------------------------------------------------------------------------

boolean connectBEN() {
  return connectOSC() && connectBenTo();
}

boolean receiveBEN() {
  receiveOSC();
  receiveBenTo();
}

//-----------------------------------------------------------------------------------
// BENTO SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP BenTo;

boolean connectBenTo() {
  return BenTo.begin(bset.benInputPort) == 1;
}

//-----------------------------------------------------------------------------------
// BENTO PACKET MANAGEMENT
//-----------------------------------------------------------------------------------

void receiveBenTo() {
  uint16_t packetSize = BenTo.parsePacket();
  if (packetSize) {
    BenTo.read(_DIN, packetSize);
    setRGB(0, (packetSize - 1) / 3, 0);
  }
}

//-----------------------------------------------------------------------------------
// YO OSC SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP OSC;

boolean connectOSC() {
  return OSC.begin(bset.oscInputPort) == 1;
}

//-----------------------------------------------------------------------------------
// YO OSC PROTOCOL
//-----------------------------------------------------------------------------------

void receiveOSC() {
  OSCMessage msg;
  uint16_t packetSize = OSC.parsePacket();
  if ( packetSize > 0)  {
    while (packetSize--) {
      char c = OSC.read();
      msg.fill(c);
    }
    if (!msg.hasError()) {
      msg.dispatch("/yo", wassup);
    }
  }
}

void wassup(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isString(0)) {
    /* BROKEN
    msg.getString(0, bset.outputIp, IP_LEN);
    OSCMessage answer("/wassup");
    answer.add(WiFi.localIP().toString());
    OSC.beginPacket(bset.outputIp, bset.oscOutputPort);
    answer.send(OSC);
    OSC.endPacket();
    answer.empty();
    */
  }
}
