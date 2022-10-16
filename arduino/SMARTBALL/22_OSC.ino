//-----------------------------------------------------------------------------------
// OSC SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP OSC;

boolean connectOSC() {
  return OSC.begin(oset.oscInputPort) == 1;
}


//-----------------------------------------------------------------------------------
// OSC PROTOCOL (EXPERIMENTAL)
//-----------------------------------------------------------------------------------

void receiveOSC() {
  OSCBundle  bdl;
  OSCMessage msg;
  uint16_t packetSize = OSC.parsePacket();
  if ( packetSize > 0)  {
    while (packetSize--) {
      char c = OSC.read();
      msg.fill(c);
      bdl.fill(c);
    }
    if (!msg.hasError()) {
      msg.dispatch("/sb/save",    oscSave);
      msg.dispatch("/sb/infra",   oscInfra);
      msg.dispatch("/sb/motor",   oscMotor);
      msg.dispatch("/sb/strobe",  oscStrobe);
      msg.dispatch("/sb/master",  oscMaster);
      msg.dispatch("/sb/color1",  oscColor1);
      msg.dispatch("/sb/color2",  oscColor2);
      msg.dispatch("/sb/portal",  oscPortal);
      msg.dispatch("/sb/connect", oscConnect);
    }
    else if (!bdl.hasError()) {
      bdl.dispatch("/sb/save",    oscSave);
      bdl.dispatch("/sb/infra",   oscInfra);
      bdl.dispatch("/sb/motor",   oscMotor);
      bdl.dispatch("/sb/strobe",  oscStrobe);
      bdl.dispatch("/sb/master",  oscMaster);
      bdl.dispatch("/sb/color1",  oscColor1);
      bdl.dispatch("/sb/color2",  oscColor2);
      bdl.dispatch("/sb/portal",  oscPortal);
      bdl.dispatch("/sb/connect", oscConnect);
    }
  }
}


//-----------------------------------------------------------------------------------
// OSC RECEIVES
//-----------------------------------------------------------------------------------

void oscSave(OSCMessage& msg) {
  saveOSCSettings();

  OSCMessage answer("/sb/saved");
  OSC.beginPacket(oset.outputIp, oset.oscOutputPort);
  answer.send(OSC);
  OSC.endPacket();
  answer.empty();
}

void oscPortal(OSCMessage& msg) {
  portalReboot();
}

void oscInfra(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isInt(0)) {
    int32_t val = constrain(msg.getInt(0), 0, 1023);
    changeIRL(val);
  }
}

void oscColor(byte slot, OSCMessage& msg) {
  uint8_t addr = 0;
  bool execute = true;
  for (int i = 0; i < msg.size(); i++) {
    execute = msg.isInt(i);
  }
  if (execute) {
    for (int i = 0; i < msg.size(); i++) {
      int32_t v = msg.getInt(i);
      _DIN[addr++] = (v >> 24) & 255;
      _DIN[addr++] = (v >> 16) & 255;
      _DIN[addr++] = (v >> 8)  & 255;
    }
  }
  setRGB(slot, msg.size(), 0);
}

void oscColor1(OSCMessage& msg) {
  oscColor(0, msg);
}

void oscColor2(OSCMessage& msg) {
  oscColor(1, msg);
}

void oscMotor(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isInt(0)) {
    int32_t val = constrain(msg.getInt(0), 0, 1023);
    changeMOT(val);
  }
}

void oscStrobe(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isFloat(0)) {
    checkSTB(msg.getFloat(0) * 100.);
  }
}

void oscMaster(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isFloat(0)) {
    checkMST(msg.getFloat(0));
  }
}

void oscConnect(OSCMessage& msg) {
  if (msg.size() == 1 && msg.isString(0)) {
    char ipString[IP_LEN];
    msg.getString(0, ipString, IP_LEN);

    IPAddress tmpIp;
    tmpIp.fromString(ipString);
    oset.outputIp = (uint32_t)tmpIp;

    char smartballIp[IP_LEN];
    WiFi.localIP().toString().toCharArray(smartballIp, IP_LEN);

    OSCMessage answer("/sb/connected");
    answer.add((int32)gset.idNumber);
    answer.add(smartballIp);
    OSC.beginPacket(oset.outputIp, oset.oscOutputPort);
    answer.send(OSC);
    OSC.endPacket();
    answer.empty();
  }
}
