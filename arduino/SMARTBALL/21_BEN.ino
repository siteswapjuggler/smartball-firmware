//-----------------------------------------------------------------------------------
// BENTO PROTOCOL
//-----------------------------------------------------------------------------------

boolean connectBEN() {
  //Serial.println("- connect BenTo");
  return connectYO() && connectBenTo();
}

boolean receiveBEN() {
  receiveYO();
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

WiFiUDP YO;

boolean connectYO() {
  return YO.begin(bset.yoInputPort) == 1;
}

//-----------------------------------------------------------------------------------
// YO OSC PROTOCOL
//-----------------------------------------------------------------------------------

void receiveYO() {
  OSCMessage msg;
  uint16_t packetSize = YO.parsePacket();
  if ( packetSize > 0)  {
    while (packetSize--) {
      char c = YO.read();
      msg.fill(c);
    }
    if (!msg.hasError()) {
      msg.dispatch("/yo", wassup);
    }
  }
}

void wassup(OSCMessage& msg) {
  //Serial.println("wassup");
  if (msg.size() == 1 && msg.isString(0)) {
    char ipString[IP_LEN];
    msg.getString(0, ipString, IP_LEN);
    
    IPAddress tmpIp;
    tmpIp.fromString(ipString);  
    bset.outputIp = (uint32_t)tmpIp;

    char smartballIp[IP_LEN];
    WiFi.localIP().toString().toCharArray(smartballIp, IP_LEN);

    OSCMessage answer("/wassup");
    answer.add(smartballIp);
    answer.add((int32)gset.idNumber);
    YO.beginPacket(bset.outputIp, bset.yoOutputPort);
    answer.send(YO);
    YO.endPacket();
    answer.empty();
  }
}
