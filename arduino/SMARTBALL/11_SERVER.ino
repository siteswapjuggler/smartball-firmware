//-----------------------------------------------------------------------------------
// WEB SERVER
//-----------------------------------------------------------------------------------

ESP8266WebServer server(80);                            //Server on port 80

void initWebServer() {
  LittleFS.begin();                                       // Start file system
  server.on("/", handleRoot);                           // Hande root request
  server.on("/testBall", testBall);                     // Test the ball
  server.on("/rebootBall", normalReboot);               // Reboot the ball in normal mode
  server.on("/getNetworks", getNetworks);               // Get available networks
  server.on("/getParameters", getParameters);           // Asking for actual parameters
  server.on("/refreshNetworks", refreshNetworks);       // Refresh available networks
  server.on("/setNetwork", HTTP_POST, setNetwork);      // Setting WiFi network parameters
  server.onNotFound(handleAny);                         // Handle not found ressources
  server.begin();
}

void updateWebServer() {
  server.handleClient();
}

//-----------------------------------------------------------------------------------
// WEBSERVER API
//-----------------------------------------------------------------------------------

bool test = false;

void testBall() {
  test = test ? false : true;
   changeRGB(0,test ? TEST_COLOR : BLACK);  
  server.send(200, "text/plain", test ? "TEST ON" : "TEST OFF");
}

void getNetworks() {
  server.send(200, "text/plain", availableNetworks);
}

void refreshNetworks() {
  availableNetworks = listNetworks();
  getNetworks();
}

void setNetwork() {
  server.arg("ssid").toCharArray(wset.ssid, SSID_LEN);
  server.arg("pwd").toCharArray(wset.password, PWD_LEN);
  saveWifiSettings();
  if (!handleFileRead("/reboot.html")) handleError;
}

void getParameters() {
  String answer;
  answer += String(wset.ssid)+","+String(gset.idNumber)+","+String(HOSTNAME)+",";
  answer += String(FIRMWARE_VERSION)+" | "+String(FIRMWARE_STATE)+","+eepromVersion+","+String(PROTOCOL_VERSION);
  server.send(200, "text/plain", answer);
}

//-----------------------------------------------------------------------------------
// RESSOURCE HANDLERS
//-----------------------------------------------------------------------------------

void handleRoot() {
  if (!handleFileRead("/"))
    handleError();
}

void handleAny() {
  if (!handleFileRead(server.uri()))
    handleError();
}

void handleError() {
  if (!handleFileRead("/404.html"))
    server.send(404, F("text/plain"), F("404: Not Found"));
}

//-----------------------------------------------------------------------------------
// FILE SYSTEM HANDLERS
//-----------------------------------------------------------------------------------

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
    if (LittleFS.exists(pathWithGz))
      path += ".gz";
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String getContentType(String filename) {
  if      (filename.endsWith(".gz"))   return "application/x-gzip";
  else if (filename.endsWith(".ico"))  return "image/x-icon";
  else if (filename.endsWith(".png"))  return "image/png";
  else if (filename.endsWith(".jpg"))  return "image/jpeg";
  else if (filename.endsWith(".gif"))  return "image/gif";
  else if (filename.endsWith(".svg"))  return "image/svg+xml";
  else if (filename.endsWith(".htm"))  return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css"))  return "text/css";
  else if (filename.endsWith(".js"))   return "application/javascript";
  else if (filename.endsWith(".xml"))  return "text/xml";
  else if (filename.endsWith(".pdf"))  return "application/x-pdf";
  else if (filename.endsWith(".zip"))  return "application/x-zip";
  return "text/plain";
}
