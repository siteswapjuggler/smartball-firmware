//-----------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//-----------------------------------------------------------------------------------

unsigned long debugTime;

void initDebug() {
#if defined(MSG_DEBUG) || defined(TIME_DEBUG)
  Serial.begin(1000000);
#endif
}

void getDebugTime() {
#ifdef TIME_DEBUG
  debugTime = micros();
#endif
}

void printTimeDebug(const char* msg) {
#ifdef TIME_DEBUG
  Serial.print(msg);
  Serial.println(micros()-debugTime);
#endif  
}

void printMsgDebug(uint8_t c, uint16_t l, uint8_t *data) {
#ifdef MSG_DEBUG
  Serial.print("CMD: " + String(c, HEX) + "\t LEN: " + String(l) + "\t DATA: ");
  for (int i = 0; i < l; i++) {
    Serial.print(String(data[i]));
    if (i + 1 < l ) Serial.print(" ");
  }
  Serial.println();
#endif
}

void printError(const char *error) {
#ifdef MSG_DEBUG
  Serial.print("ERROR: ");
  Serial.println(error);
#endif
}
