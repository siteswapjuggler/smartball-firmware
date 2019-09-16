/* ----------------------------------------------------------------------------------
   THE SMARTBALL PROJECT - 11/05/2019
   Copyright 2013-2019 Sylvain GARNAVAULT
   ----------------------------------------------------------------------------------

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.

   ----------------------------------------------------------------------------------

   Written as part of the Smartball project:
    - https://github.com/siteswapjuggler/smartball-hardware
    - https://github.com/siteswapjuggler/smartball-firmware
    - https://github.com/siteswapjuggler/smartball-externals
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
#include <ESP8266WebServer.h>  // allow WebServer for configuration
#include <WiFiUDP.h>           // UDP Protocol Library
#include <OSCBundle.h>         // OSC Protocol Library
#include <MPU9250.h>           // Custom MPU920 SPI Library - see libraries subfolder
#include <Adafruit_DotStar.h>  // Adafruit_Dotstar Library - https://github.com/adafruit/Adafruit_DotStar >> Issue with SPI Mode has been documented here https://github.com/adafruit/Adafruit_DotStar/isetsues/28

//-----------------------------------------------------------------------------------
// USER PARAMETERS
//-----------------------------------------------------------------------------------

//#define DEBUG                // uncomment for serial debugging messages

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

bool   serverMode;
Ticker batTicker;              // battery management ticker
Ticker imuTicker;              // imu management ticker
Ticker frameTicker;            // main frame ticker

//-----------------------------------------------------------------------------------
// INITIAL SETUP
//-----------------------------------------------------------------------------------

void setup() {
  blinkRGB(RED, 250, 125);
  initDebug();
  initEEPROM();
  if (imuAvailable()) initIMU();
  if (rgbAvailable()) initRGB();
  if (irlAvailable()) initIRL();
  if (motAvailable()) initMOT();
  if (connectWifi()) {
    serverMode = false;
    blinkRGB(BLUE, 250, 125);
    if (connectDGM())   blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
    if (connectOSC())   blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
    if (connectBenTo()) blinkRGB(GREEN, 250, 125); else blinkRGB(RED, 250, 125);
  }
  else {
    serverMode = true;
    accessPointInit();
    serverInit();
  }
  imuTicker.attach_ms(7, imuFrame);       // IMU updates @ 142.85 Hz
  batTicker.attach_ms(100, updateBAT);    // BAT updates @ 10 Hz
  frameTicker.attach_ms(10, mainFrame);   // Main updates @ 100 Hz
}

//-----------------------------------------------------------------------------------
// MAIN LOOP
//-----------------------------------------------------------------------------------

void loop() {
  if (serverMode) {
    serverUpdate();                       // (optional) Webserver Management
  }
  else {
    strobeUpdate();                       // update strobe state
    receiveDGM();                         // receive Smartball Datagrams
    receiveOSC();                         // receive Yo Protocol
    receiveBenTo();                       // receive BenTo Datagrams
  }
}

//-----------------------------------------------------------------------------------
// FRAME TICKER CALLBACK
//-----------------------------------------------------------------------------------

void mainFrame() {
  if (rgbAvailable()) {
    updateSTB();                        // update Strobe speed
    updateRGB();                        // update RGB leds
  }
  if (irlAvailable()) updateIRL();      // update infrared leds values
  if (motAvailable()) updateMOT();      // update vibration motor values
}

//-----------------------------------------------------------------------------------
// IMU TICKER CALLBACK
//-----------------------------------------------------------------------------------

void imuFrame() {
 if (imuAvailable()) updateIMU();      // update IMU values
}
