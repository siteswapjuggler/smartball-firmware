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
      msg.dispatch("/wifiSettings", wifiSettings);
      msg.dispatch("/saveWifiSettings", saveWifiSettings);
      msg.dispatch("/oscReboot", oscReboot);
    }
  }
}

//-----------------------------------------------------------------------------------
// PROTOCOL ANSWER
//-----------------------------------------------------------------------------------

void wassup(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isString(0)) {
    msg.getString(0, wset.outputIp, IP_LEN);

    char smartballIP[32];
    WiFi.localIP().toString().toCharArray(smartballIP, 32);

    OSCMessage answer("/wassup");
    answer.add(smartballIP);
    answer.add((int32)fset.serialNumber);
    OSC.beginPacket(wset.outputIp, OSC_OUT);
    answer.send(OSC);
    OSC.endPacket();
    answer.empty();
  }
}

void wifiSettings(OSCMessage& msg) {
  if (msg.size() == 3 && msg.isString(0) && msg.isString(1)) {
    msg.getString(0, wset.ssid, SSID_LEN);
    msg.getString(1, wset.password, PWD_LEN);
    msg.getString(2, wset.outputIp, IP_LEN);
  }
}

void saveWifiSettings(OSCMessage& msg) {
    saveWifiSettings();
}

void oscReboot(OSCMessage& msg) {
    espReboot();
}
