//-----------------------------------------------------------------------------------
// UDP SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP OSC;                       // OSC UDP socket
#define OSC_IN   9000              // OSC UDP in port
#define OSC_OUT 10000              // OSC UDP out port

boolean connectOSC() {
  return OSC.begin(OSC_IN) == 1;
}

//-----------------------------------------------------------------------------------
// PACKET MANAGEMENT
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
  
//-----------------------------------------------------------------------------------
// PROTOCOL ANSWER
//-----------------------------------------------------------------------------------

void wassup(OSCMessage& msg) {
   if (msg.size() == 1 && msg.isString(0)) {
    char clientIP[32];
    msg.getString(0,clientIP,32);

    char smartballIP[32];
    WiFi.localIP().toString().toCharArray(smartballIP,32);
    
    OSCMessage answer("/wassup");
    answer.add(smartballIP);
    answer.add((int32)fset.serialNumber);
    OSC.beginPacket(clientIP, OSC_OUT);
    answer.send(OSC);
    OSC.endPacket();
    answer.empty();
  }
}
