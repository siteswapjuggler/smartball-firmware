//-----------------------------------------------------------------------------------
// WIFI CONNECTION & LISTING
//-----------------------------------------------------------------------------------

char* HOSTNAME;
String availableNetworks;

boolean connectWifi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  availableNetworks = listNetworks();
  setHostname(fset.serialNumber);
  WiFi.begin(wset.ssid, wset.password);
  bool state = true;
  uint32_t timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    timeout += blinkLed(WAIT_COLOR, QUICK_BLINK);
    if (timeout >= WIFI_TIMEOUT || WiFi.status() == WL_CONNECT_FAILED) {
      state = false;
      WiFi.disconnect();
      for (byte n = 0; n < 4; n++) blinkLed(ALERT_COLOR, LONG_BLINK);
      break;
    }
  }
  return state;
}

String listNetworks() {
  String options;
  int n = WiFi.scanNetworks();
  if (n == 0)
    options += "false";
  else {
    for (int i = 0; i < n; ++i) {
      options += WiFi.SSID(i);
      if (i < n - 1) options += ",";
    }
  }
  return options;
}

//----------------------------------------------------------------------------------------
// ACCESS POINT CONFIGURATION
//----------------------------------------------------------------------------------------

IPAddress apIP(192, 168, 1, 1);
IPAddress mask(255, 255, 255, 0);

void initAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, mask);
  WiFi.softAP(HOSTNAME);
}

void setHostname(uint16_t sn) {
  if (sn) {
    HOSTNAME = "SMARTBALL_00000";
    HOSTNAME = "SB_00000";
    for (int i = 0, m = 1; i < 5; i++) {
      HOSTNAME[7 - i] = 48 + ((sn / m) % 10);
      m *= 10;
    }
  }
  else {
    HOSTNAME = "SB_ALPHA";
  }
}

//----------------------------------------------------------------------------------------
// DNS CONFIGURATION
//----------------------------------------------------------------------------------------

#define DNS_PORT 53
DNSServer dnsServer;

void initDNS() {
  dnsServer.start(DNS_PORT, "setup.smartball.io", apIP);
}

void updateDNS() {
  dnsServer.processNextRequest();
}

//----------------------------------------------------------------------------------------
// MDNS CONFIGURATION
//----------------------------------------------------------------------------------------

void initMDNS() {
  MDNS.begin(HOSTNAME);
  if (!serverMode) {
    MDNS.addService("datagram", "udp", 8000);
    MDNS.addService("bento", "udp", 8888);
    MDNS.addService("osc", "udp", 9000);
  }
  else {
    MDNS.addService("http","tcp", 80);
  }
}

void updateMDNS() {
  MDNS.update();
}

//-----------------------------------------------------------------------------------
// EEPROM FUNCTIONS
//-----------------------------------------------------------------------------------

void saveWifiSettings() {
  EEPROM.begin(512);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();
}
