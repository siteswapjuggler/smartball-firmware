//-----------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//-----------------------------------------------------------------------------------

void initDebug() {
#ifdef DEBUG
  Serial.begin(1000000);
#endif
}

void printDebug(uint8_t c, uint16_t l, uint8_t *data) {
#ifdef DEBUG
  Serial.print("CMD: " + String(c, HEX) + "\t LEN: " + String(l) + "\t DATA: ");
  for (int i = 0; i < l; i++) {
    Serial.print(String(data[i]));
    if (i + 1 < l ) Serial.print(" ");
  }
  Serial.println();
#endif
}

void printError(const char *error) {
#ifdef DEBUG
  Serial.print("ERROR: ");
  Serial.println(error);
#endif
}
