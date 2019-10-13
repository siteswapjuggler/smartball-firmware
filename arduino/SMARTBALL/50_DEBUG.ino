//-----------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//-----------------------------------------------------------------------------------

void initDebug() {
#if defined(DGM_DEBUG) || defined(TIME_DEBUG)
  Serial.begin(115200);
#endif
}

//-----------------------------------------------------------------------------------
// TIME MEASURE
//-----------------------------------------------------------------------------------

unsigned long debugTime;

void getDebugTime() {
#ifdef TIME_DEBUG
  debugTime = micros();
#endif
}

void printTimeDebug(const char* msg, bool ln) {
#ifdef TIME_DEBUG
  uint32_t delta = micros() - debugTime;
  Serial.print(msg);
  Serial.print(delta);
  if (ln) Serial.println();
#endif
}

//-----------------------------------------------------------------------------------
// DATAGRAM DEBUG
//-----------------------------------------------------------------------------------

void printDgmDebug(uint8_t c, uint16_t l, uint8_t *data) {
#ifdef DGM_DEBUG
  Serial.print("CMD: " + String(c, HEX) + "\t LEN: " + String(l));
  if (l) {
    /*Serial.print("\t DATA: ");
    for (int i = 0; i < l; i++) {
      Serial.print(String(data[i]));
      if (i + 1 < l ) Serial.print(" ");
    }*/
    if (c == 33) {
      Serial.print("\t NUM: ");
      unsigned long n = data[7] << 16 | data[8] << 8 | data[9];
      Serial.print(n);
    }
  }
  Serial.println();
#endif
}

void printError(const char *error) {
#ifdef DGM_DEBUG
  Serial.print("ERROR: ");
  Serial.println(error);
#endif
}
