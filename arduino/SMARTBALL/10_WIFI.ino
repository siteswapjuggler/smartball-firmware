//-----------------------------------------------------------------------------------
// WIFI CONNECTION
//-----------------------------------------------------------------------------------

char* HOSTNAME;

boolean connectWifi() {
  WiFi.mode(WIFI_STA);
  setHostname(fset.serialNumber);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(wset.ssid, wset.password);
  int i = 0;
  bool state = true;
  while (WiFi.status() != WL_CONNECTED) {
    blinkRGB(WAIT, 125, 125);
    if (i > 300 || WiFi.status() == WL_CONNECT_FAILED) {
      state = false;
      WiFi.disconnect();
      for (byte n = 0; n < (4 * 6); n++) blinkRGB(ALERT, 125, 125);
      break;
    }
    i++;
  }
  return state;
}

//-----------------------------------------------------------------------------------
// WIFI SERVICES
//-----------------------------------------------------------------------------------

void accessPointInit() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(HOSTNAME);
}

void setHostname(uint16_t sn) {
  if (sn) {
    HOSTNAME = "SMARTBALL_00000";
    for (int i = 0, m = 1; i < 5; i++) {
      HOSTNAME[14 - i] = 48 + ((sn / m) % 10);
      m *= 10;
    }
  }
  else {
    HOSTNAME = "ALPHA";
  }
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveWifiSettings() {
  EEPROM.begin(512);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}
