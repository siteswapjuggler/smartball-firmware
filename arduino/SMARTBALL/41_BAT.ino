//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

float batValue   = 0.;
int   batCounter = 0;

//-----------------------------------------------------------------------------------
// BASIC FUNCTIONS
//-----------------------------------------------------------------------------------

void updateBAT() {
  // REFRESH BATT VALUE @10Hz
  float v = (float)analogRead(BATT_PIN);
  float s = fset.adcCalibration >= 0. ? fset.adcCalibration : 5.;
  float n = 5.*v / s;

  // LOW PASS FILTER FOR BATTERY VALUE
  batValue *= 0.2;
  batValue += 0.8 * n;

  // SEND BATT VALUE @1Hz
  if (batCounter == 0) {
    if (dgm) sendBAT();
    if (osc) sendOSCBat();
  }
  batCounter++;
  batCounter %= 10;
}


//-----------------------------------------------------------------------------------
// DATAGRAM FEEDBACK
//-----------------------------------------------------------------------------------

void sendBAT() {
  int16_t val = (int16_t)(batValue * 100.);
  _DOUT[0] = val >> 8; _DOUT[1] = val & 255;
  sendDgmAnswer(CMD_BAT, 2);
}

//-----------------------------------------------------------------------------------
// OSC FEEDBACK
//-----------------------------------------------------------------------------------

void sendOSCBat() {
  OSCMessage msg("/sb/batt");
  msg.add(batValue);
  OSC.beginPacket(oset.outputIp, oset.oscOutputPort);
  msg.send(OSC);
  OSC.endPacket();
  msg.empty();
}
