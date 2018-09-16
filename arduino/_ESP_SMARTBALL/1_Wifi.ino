//---------------------------------------------------------------
// WIFI CONNECTION
//---------------------------------------------------------------

char*       HOSTNAME;

boolean connectWifi() {
  WiFi.mode(WIFI_STA);
  if (factorySettings.serialNumber) {
    HOSTNAME = "SMARTBALL_00000";
    for (int i = 0, m = 1; i < 5; i++) {
      HOSTNAME[14 - i] = 48 + ((factorySettings.serialNumber/m) % 10);
      m *= 10;
    }
  }
  else {
    HOSTNAME = "ALPHA";
  }
  WiFi.hostname(HOSTNAME);
  WiFi.begin(NETWORK, PASSWORD);

  bool state = true;
  int i = 0;

  while (WiFi.status() != WL_CONNECTED) {
    blinkRGB(0X8F4E00, 125);      // Trying connection
    blinkRGB(0X000000, 125);      
    delay(250);
    
    if (i > 120) {                // 60 seconds timeout
      state = false;
      for(byte n=0;n<(4*6);n++){
        blinkRGB(0X7F0000, 125);  // Warning
        blinkRGB(0X000000, 125);  // Warning
      }
      //espDeepSleep();           // Power saving
      break;
    }
    i++;
  }
  MDNS.begin(HOSTNAME);
  return state;
}

//---------------------------------------------------------------
// UDP SERVICES
//---------------------------------------------------------------

WiFiUDP UDP;

boolean connectUDP() {
  #ifdef UNIQUE_OUT_PORT
  dgmOutPort += factorySettings.serialNumber;  
  #endif
  if (UDP.beginMulticast(WiFi.localIP(), multiIp, dgmInPort) == 1) return true;
  else return false;
}

//---------------------------------------------------------------
// OTA SERVICE
//---------------------------------------------------------------

void connectOTA() {
#ifdef HANDLE_OTA 
  ArduinoOTA.setHostname(HOSTNAME);
  //ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    //feedback, SPIFFS end etc... here...
  });
  ArduinoOTA.onEnd([]() {});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});  
  ArduinoOTA.onError([](ota_error_t error) {});
  ArduinoOTA.begin();
#endif
}

void handleOTA() {
#ifdef HANDLE_OTA
  ArduinoOTA.handle(); 
#endif 
}

