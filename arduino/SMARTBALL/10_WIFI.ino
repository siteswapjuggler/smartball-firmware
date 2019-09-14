//-----------------------------------------------------------------------------------
// WIFI CONNECTION
//-----------------------------------------------------------------------------------

char* HOSTNAME;

boolean connectWifi() {
  setHostname(fset.serialNumber);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.config(wset.staticIp,wset.gateway,wset.subnet);
  WiFi.begin(wset.ssid, wset.password);
  uint32_t timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    timeout += blinkLed(WAIT_COLOR, QUICK_BLINK);
    if (timeout >= WIFI_TIMEOUT || WiFi.status() == WL_CONNECT_FAILED) {
      WiFi.disconnect();
      for (byte n = 0; n < 4; n++) blinkLed(ALERT_COLOR, LONG_BLINK);
      return false;
    }
  }
  return true;
}

//-----------------------------------------------------------------------------------
// WIFI HELPERS
//-----------------------------------------------------------------------------------

void setHostname(uint16_t sn) {
  if (sn) {
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
