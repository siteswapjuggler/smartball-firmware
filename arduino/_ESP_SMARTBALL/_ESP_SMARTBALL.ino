//---------------------------------------------------------------
// BOARD VERSION
//---------------------------------------------------------------

#define BOARD_VERSION 1        // 1 - Alpha and first batch for les Objets Volants

//---------------------------------------------------------------
// LIBRARIES
//---------------------------------------------------------------

#include "def/smartball.h"     // Smartball specific definitions
#include <Ticker.h>            // ESP8266 scheduling library for regular event
#include <EEPROM.h>            // 512 byte EEPROM Emulation for ESP8266
#include <ESP8266WiFi.h>       // Standard WiFi Library
#include <ESP8266mDNS.h>       // mDNS hostname standard
#include <ArduinoOTA.h>        // allow Over the Air firmware updates
#include <WiFiUDP.h>           // UDP Protocol Library
#include <MPU9250.h>           // Custom MPU920 SPI Library - see libraries subfolder
#include <Adafruit_DotStar.h>  // Adafruit_Dotstar Library - https://github.com/adafruit/Adafruit_DotStar >> Issue with SPI Mode has been documented here https://github.com/adafruit/Adafruit_DotStar/issues/28

//---------------------------------------------------------------
// USER PARAMETERS
//---------------------------------------------------------------

//#define HANDLE_OTA               // uncomment to activate OTA (experimental option)
//#define UNIQUE_OUT_PORT          // uncomment to set a unique out port for each ball, outPort = dmgOutPort + serial.number
//#define DEBUG                    // uncomment for serial debugging messages

#define NETWORK  "---------"       // your network SSID
#define PASSWORD "---------"       // your network password

IPAddress outIp(239, 0, 0, 51);    // multicast address of client applications
IPAddress multiIp(239, 0, 0, 50);  // multicast address of smartballs

unsigned int dgmInPort  = 8000;    // udp datagram in port
unsigned int dgmOutPort = 9000;    // udp datagram out port

//---------------------------------------------------------------
// GLOBAL VARIABLE
//---------------------------------------------------------------

Ticker batTicker;
Ticker frameTicker;

//---------------------------------------------------------------
// INITIAL SETUP 
//---------------------------------------------------------------

unsigned long previousTime;

void setup() {
  initDebug();
  initEEPROM();
  if (imuAvailable()) initIMU();
  if (rgbAvailable()) initRGB();
  if (irlAvailable()) initIRL();
  if (motAvailable()) initMOT();
  if (connectWifi()) {
    connectOTA();
    blinkRGB(BLUE, 500);
    if (connectUDP()) blinkRGB(GREEN, 500);
    else while (true);
  }
  batTicker.attach_ms(100, updateBAT);
  frameTicker.attach_ms(10, mainFrame);
}

//---------------------------------------------------------------
// MAIN LOOP
//---------------------------------------------------------------

void loop() {
  receiveDatagram();
  handleOTA();
}

void mainFrame() {
  if (rgbAvailable()) updateRGB();
  if (imuAvailable()) updateIMU();
  if (irlAvailable()) updateIRL();
  if (motAvailable()) updateMOT();
}

