//-----------------------------------------------------------------------------------
// WEB SERVER
//-----------------------------------------------------------------------------------

ESP8266WebServer server(80);                            //Server on port 80

void serverInit() {
  SPIFFS.begin();                                       // Start file system
  server.on("/", handleRoot);                           // Which routine to handle at root location. This is display page
  server.on("/scanNetwork", scanNetwork);               // Asking for available networks
  server.on("/setNetwork", HTTP_POST, setNetwork);      // Setting WiFi network parameters
  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server.begin();
  serverMode = true;
}

void serverUpdate() {
  if (serverMode) {
    server.handleClient();
  }
}

//-----------------------------------------------------------------------------------
// RESSOURCE HANDLERS
//-----------------------------------------------------------------------------------

void handleRoot() {
  if (!handleFileRead("/"))                             // send it if it exists
    server.send(404, "text/plain", "404: Not Found");   // otherwise, respond with a 404 (Not Found) error
}

void scanNetwork() {
  String options;
  int n = WiFi.scanNetworks();
  if (n == 0)
    options += "<option disabled selected>No network found...</option>";
  else {
    for (int i = 0; i < n; ++i)
      options += "<option>" + WiFi.SSID(i) + "</option>";
  }
  server.send(200, "text/plain", options);
}

void setNetwork() {
  server.arg("ssid").toCharArray(wset.ssid, SSID_LEN);
  server.arg("pwd").toCharArray(wset.password, PWD_LEN);

  
  if (!handleFileRead("/reboot.html"))                     // send it if it exists
    server.send(404, "text/plain", "404: Not Found");      // otherwise, respond with a 404 (Not Found) error

  saveWifiSettings();
  espReboot();
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.html";            // If a folder is requested, send the index file
  String contentType = getContentType(path);               // Get the MIME type
  if (SPIFFS.exists(path)) {                               // If the file exists, either as a compressed archive, or normal
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    return true;
  }
  return false;
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js"))  return "application/javascript";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".png")) return "image/png";
  return "text/plain";
}
