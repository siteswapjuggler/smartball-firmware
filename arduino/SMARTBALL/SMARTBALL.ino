/* ----------------------------------------------------------------------------------
   THE SMARTBALL PROJECT - 29/08/2019
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
#include "SMARTBALL_DEF.h"     // Smartball specific definitions

//-----------------------------------------------------------------------------------
// LIBRARIES
//-----------------------------------------------------------------------------------

#include <FS.h>                // SPIFFS management
#include <Ticker.h>            // ESP8266 scheduling library for regular event
#include <EEPROM.h>            // 512 byte EEPROM Emulation for ESP8266
#include <ESP8266WiFi.h>       // Standard WiFi Library
#include <DNSServer.h>         // DNS Server management
#include <ESP8266mDNS.h>       // mDNS Hostname management
#include <ESP8266WebServer.h>  // allow WebServer for configuration
#include <WiFiUDP.h>           // UDP Protocol Library
#include <OSCBundle.h>         // OSC Protocol Library
#include <MPU9250.h>           // Custom MPU920 SPI Library - see libraries subfolder
#include <Adafruit_DotStar.h>  // Adafruit_Dotstar Library - https://github.com/adafruit/Adafruit_DotStar >> Issue with SPI Mode has been documented here https://github.com/adafruit/Adafruit_DotStar/isetsues/28

//-----------------------------------------------------------------------------------
// USER PARAMETERS
//-----------------------------------------------------------------------------------

#define DGM_DEBUG            // uncomment for datagram communication serial feedbacks
#define TIME_DEBUG           // uncomment for time debug serial feedbacks
#define EEPROM_DEBUG         // uncomment for EEPROM serial print during setup

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

bool   serverMode;
Ticker batTicker;              // battery management ticker
Ticker imuTicker;              // imu management ticker
Ticker sendTicker;             // data sending ticker
Ticker frameTicker;            // main frame ticker

//-----------------------------------------------------------------------------------
// INITIAL SETUP
//-----------------------------------------------------------------------------------

void setup() {
  blinkLed(BLUE, LONG_BLINK);      // (TODO visual feedback become optionnal)
  initDebug();
  initEEPROM();
  
  if (imuAvailable()) initIMU();
  if (rgbAvailable()) initRGB();
  if (irlAvailable()) initIRL();
  if (motAvailable()) initMOT();
  
  if (connectWifi()) {
    serverMode = false;
    blinkLed(BLUE, MEDIUM_BLINK);
    blinkLed(connectDGM()   ? GREEN : RED, MEDIUM_BLINK);
    blinkLed(connectOSC()   ? GREEN : RED, MEDIUM_BLINK);
    blinkLed(connectBenTo() ? GREEN : RED, MEDIUM_BLINK);
  }
  else {
    serverMode = true;
    initAccessPoint();
    initWebServer();
    initDNS();
  }

  initMDNS();
  
  if (imuAvailable()) {
    imuTicker.attach_ms(5, updateIMU);    // IMU updates  @ 200 Hz   (TODO quicker but what is the limitation)
    sendTicker.attach_ms(10, sendIMU);    // IMU sendings @ 100 Hz   (TODO choose bandwidth)
  }
  batTicker.attach_ms(100, updateBAT);    // BAT updates  @ 10 Hz    (TODO become optionnal)
  frameTicker.attach_ms(10, mainFrame);   // Main updates @ 100 Hz
}

//-----------------------------------------------------------------------------------
// MAIN LOOP
//-----------------------------------------------------------------------------------

void loop() {
  updateMDNS();                           // mDNS service responder update

  if (!serverMode) {
    updateStrobe();                       // update strobe state
    receiveDGM();                         // receive Smartball Datagrams (TODO become optional)
    receiveOSC();                         // receive Yo Protocol         (TODO become optional)
    receiveBenTo();                       // receive BenTo Datagrams     (TODO become optional)
  }  
  else {
    updateDNS();                          // (optional) DNS redirection service
    updateWebServer();                    // (optional) Webserver Management
  }
  
  yield();                                // manage WiFi tasks
  ESP.wdtFeed();                          // feeding watchdog to prevent unexpected reboot
}

//-----------------------------------------------------------------------------------
// FRAME TICKER CALLBACK
//-----------------------------------------------------------------------------------

void mainFrame() {
  if (rgbAvailable()) {
    updateSTB();                        // update Strobe speed
                                        // TODO insert mode management here
    updateRGB();                        // update RGB leds
  }
  if (irlAvailable()) updateIRL();      // update infrared leds values
  if (motAvailable()) updateMOT();      // update vibration motor values
}
