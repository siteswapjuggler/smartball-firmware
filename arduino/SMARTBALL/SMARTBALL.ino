/* ----------------------------------------------------------------------------------
 * THE SMARTBALL PROJECT - 23/09/2018
 * Copyright 2013-2018 Sylvain GARNAVAULT 
 * ----------------------------------------------------------------------------------
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * ----------------------------------------------------------------------------------
 * 
 * Written as part of the Smartball project:
 *  - https://github.com/siteswapjuggler/smartball-hardware
 *  - https://github.com/siteswapjuggler/smartball-firmware 
 *  - https://github.com/siteswapjuggler/smartball-externals
 */

//-----------------------------------------------------------------------------------
// BOARD VERSION
//-----------------------------------------------------------------------------------

#define BOARD_VERSION 1        // 1 - Alpha and first batch for les Objets Volants
#include "header/smartball.h"  // Smartball specific definitions

//-----------------------------------------------------------------------------------
// LIBRARIES
//-----------------------------------------------------------------------------------

#include <FS.h>                // SPIFFS management
#include <Ticker.h>            // ESP8266 scheduling library for regular event
#include <EEPROM.h>            // 512 byte EEPROM Emulation for ESP8266
#include <ESP8266WiFi.h>       // Standard WiFi Library
#include <ESP8266mDNS.h>       // mDNS hostname standard
#include <ArduinoOTA.h>        // allow Over the Air firmware updates
#include <ESP8266WebServer.h>  // allow WebServer for configuration
#include <WiFiUDP.h>           // UDP Protocol Library
#include <OSCBundle.h>         // OSC Protocol Library
#include <MPU9250.h>           // Custom MPU920 SPI Library - see libraries subfolder
#include <Adafruit_DotStar.h>  // Adafruit_Dotstar Library - https://github.com/adafruit/Adafruit_DotStar >> Issue with SPI Mode has been documented here https://github.com/adafruit/Adafruit_DotStar/isetsues/28

//-----------------------------------------------------------------------------------
// USER PARAMETERS
//-----------------------------------------------------------------------------------

//#define HANDLE_OTA           // uncomment to activate OTA (experimental option)
//#define UNIQUE_OUT_PORT      // uncomment to set a unique out port for each ball, outPort = dmgOutPort + serial.number
//#define DEBUG                // uncomment for serial debugging messages

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

Ticker batTicker;              // battery management ticker
Ticker frameTicker;            // main frame ticker

//-----------------------------------------------------------------------------------
// INITIAL SETUP
//-----------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  initDebug();
  initEEPROM();
  if (imuAvailable()) initIMU();
  if (rgbAvailable()) initRGB();
  if (irlAvailable()) initIRL();
  if (motAvailable()) initMOT();
  if (connectWifi()) {
    serverInit();
    connectOTA();
//    connectMDNS(); // Does not work with the server
    blinkRGB(BLUE, 250, 125);
    if (connectDGM())   blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
    if (connectOSC())   blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
    if (connectBenTo()) blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
  }
  else {
    accessPointInit();
    serverInit();
  }
  batTicker.attach_ms(100, updateBAT);
  frameTicker.attach_ms(10, mainFrame);
}

//-----------------------------------------------------------------------------------
// MAIN LOOP
//-----------------------------------------------------------------------------------

void loop() {
  strobeUpdate();                       // update strobe state
  receiveDGM();                         // receive Smartball Datagrams
  receiveOSC();                         // receive Yo Protocol
  receiveBenTo();                       // receive BenTo Datagrams
  handleOTA();                          // (optional) OverTheAir Management
  serverUpdate();                       // (optional) Webserver Management
}

//-----------------------------------------------------------------------------------
// FRAME TICKER CALLBACK
//-----------------------------------------------------------------------------------

void mainFrame() {
  if (rgbAvailable()) {
    updateSTB();                        // update Strobe speed
    updateRGB();                        // update RGB leds
  }
  if (imuAvailable()) updateIMU();      // update IMU values
  if (irlAvailable()) updateIRL();      // update infrared leds values
  if (motAvailable()) updateMOT();      // update vibration motor values
}

