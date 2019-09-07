/* -------------------------------------------------------------------------------------
   THE SMARTBALL PROJECT - 08/09/2019
   Copyright 2013-2019 Sylvain GARNAVAULT
   -------------------------------------------------------------------------------------

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.

   -------------------------------------------------------------------------------------

   Written as part of the Smartball project:
    - https://github.com/siteswapjuggler/smartball-hardware
    - https://github.com/siteswapjuggler/smartball-firmware
    - https://github.com/siteswapjuggler/smartball-externals
*/

//--------------------------------------------------------------------------------------
// BOARD AND VERSION INFO
//--------------------------------------------------------------------------------------

#define BOARD_VERSION 1        // 1 - Alpha and first batch for les Objets Volants
#include "SMARTBALL_DEF.h"     // Smartball specific definitions

//--------------------------------------------------------------------------------------
// USER PARAMETERS
//--------------------------------------------------------------------------------------

//#define DGM_DEBUG            // uncomment for datagram communication serial feedbacks
//#define TIME_DEBUG           // uncomment for time debug serial feedbacks
//#define EEPROM_DEBUG         // uncomment for EEPROM serial print during setup

//--------------------------------------------------------------------------------------
// INITIAL SETUP
//--------------------------------------------------------------------------------------

void setup() {
  //------------------------------------------------------------------------------------
  // hardware configuration
  //------------------------------------------------------------------------------------

  //resetSettings();

  initDebug();                  // serial connection
  initEeprom();                 // get EEPROM datas
  initDevice();                 // hardware initialisation
  initSystem();                 // system initialisation

  blinkLed(BLUE, LONG_BLINK);   // end of hardware config

  //------------------------------------------------------------------------------------
  // WiFi configuration
  //------------------------------------------------------------------------------------

  if (connectWifi()) {
    if (dgm) blinkLed(connectDGM() ? GREEN : RED, MEDIUM_BLINK);
    if (ben) blinkLed(connectBEN() ? GREEN : RED, MEDIUM_BLINK);
    operationMode = RUN;
  }
  else {
    initAccessPoint();
    initWebServer();
    initDNS();
    operationMode = SET;
  }

  //------------------------------------------------------------------------------------
  // Task configuration
  //------------------------------------------------------------------------------------

  switch (operationMode) {
    case RUN:
      frameTicker.attach_ms(10, mainFrame);            // Main updates @ 100 Hz
      if (bat) batTicker.attach_ms(100, updateBAT);    // BAT updates  @  10 Hz
      if (imu) imuTicker.attach_ms(5, updateIMU);      // IMU updates  @ 200 Hz
      break;
    case SET:
      availableNetworks = listNetworks();
      break;
  }
}

//--------------------------------------------------------------------------------------
// MAIN LOOP
//--------------------------------------------------------------------------------------

void loop() {
  switch (operationMode) {
    case RUN:
      if (rgb) updateStrobe();              // update strobe state
      if (dgm) receiveDGM();                // receive Smartball Datagrams
      if (ben) receiveBEN();                // receive BenTo & Yo Protocol
      break;
    case SET:
      updateDNS();                          // DNS redirection service
      updateWebServer();                    // Webserver Management
      break;
    }
  yield();                                  // WiFi tasks extra management
  ESP.wdtFeed();                            // feeding watchdog
}

//--------------------------------------------------------------------------------------
// FRAME TICKER CALLBACK
//--------------------------------------------------------------------------------------

void mainFrame() {
  if (rgb) {
    updateSTB();           // update Strobe speed
    updateRGB();           // update RGB leds
  }
  if (irl) updateIRL();    // update infrared leds values
  if (mot) updateMOT();    // update vibration motor values
}

//--------------------------------------------------------------------------------------
// SYSTEM FUNCTIONS
//--------------------------------------------------------------------------------------

void espReboot() {
  ESP.restart();
}
