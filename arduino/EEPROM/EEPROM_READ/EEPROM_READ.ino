#include <EEPROM.h>

#define EEPROM_VERSION 2
#include "smartball_eeprom.h"

void setup() {
  Serial.begin(115200);
  getParameters();
}

void loop() {
  printParameters();
  for (byte i=0; i<5; i++) {
    Serial.println(".");
    delay(1000);
  }
  Serial.println();
}
