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
  float v = (float)analogRead(BATT_PIN);    // TODO COULD BE A PROBLEM FOR WIFI ROUTINE
  float s = fset.adcCalibration >= 0. ? fset.adcCalibration : 5.;
  float n = 5.*v/s;
  
  // LOW PASS FILTER FOR BATTERY VALUE
  batValue *= 0.2;
  batValue += 0.8*n;

  // SEND BATT VALUE @1Hz
  if (batCounter == 0) {
    int16_t val = (int16_t)(batValue * 100.);
    _DOUT[0] = val >> 8; _DOUT[1] = val & 255;
    sendDgmAnswer(CMD_BAT,2);
  }
  batCounter++;
  batCounter %= 10;
}
