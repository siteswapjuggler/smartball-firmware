//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

int motValue = 0;

//---------------------------------------------------------------
// BASIC FUNCTIONS
//---------------------------------------------------------------

void initMOT() {
  motValue = 0;
  pinMode(MOT_PIN, OUTPUT);
  updateMOT();
}

void updateMOT() {
  analogWrite(MOT_PIN, motValue);
}

bool motAvailable() {
  return factorySettings.deviceFlag & (1 << MOT_BIT);
}

bool changeMOT(int16_t v) {
  motValue = v & 1023;
}

//---------------------------------------------------------------
// PROTOCOL FUNCTIONS
//---------------------------------------------------------------

void setMOT() {
  changeMOT(_DIN[0] << 8 | _DIN[1]);
}
