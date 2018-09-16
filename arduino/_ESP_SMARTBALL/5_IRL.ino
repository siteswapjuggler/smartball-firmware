//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

int irlValue = 0;

//---------------------------------------------------------------
// BASIC FUNCTION
//---------------------------------------------------------------

void initIRL() {
  irlValue = 0;
  pinMode(IRL_PIN, OUTPUT);
  updateIRL();
}

void updateIRL() {
  analogWrite(IRL_PIN, irlValue);
}

bool irlAvailable() {
  return factorySettings.deviceFlag & (1 << IRL_BIT);
}

void changeIRL(int16_t v) {
  irlValue = v & 1023;
}

//---------------------------------------------------------------
// PROTOCOL FUNCTIONS
//---------------------------------------------------------------

void setIRL() {
  changeIRL(_DIN[0] << 8 | _DIN[1]);
}
