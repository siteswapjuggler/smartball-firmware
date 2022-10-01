 /* -------------------------------------------------------------------------------------
   THE SMARTBALL PROJECT - 09/09/2019
   Copyright 2018-2019 Sylvain GARNAVAULT
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

//------------------------------------------
// MACROS, CONSTANTS AND LIBRARIES
//------------------------------------------

#include <ESP8266WiFi.h>
#include <SB_MPU9250.h>
#include <Adafruit_DotStar.h>

#define BOARD_VERSION 2

#define NUM_LED   6
#define IRL_PIN   D1
#define MOT_PIN   D2
#define IMU_CS    D3

#if !defined(BOARD_VERSION) || BOARD_VERSION == 1
#define RGB_CS    D0
#elif BOARD_VERSION == 2
#define INT_PIN   D0
#define RGB_CS    D4
#define BUZ_PIN   D8
#else
#error "Invalid board version"
#endif

MPU9250 IMU(SPI, IMU_CS);
Adafruit_DotStar strip = Adafruit_DotStar(NUM_LED,DOTSTAR_BGR);

#define SPEED    500
#define VALUE    420
#define FREQ     880

//******************************************************************************************

void setup() {
  Serial.begin(115200);
  initIMU();
  initLED();
//  fullLED();
  initMOT();
  initIRL();
  initBUZ();
}

//******************************************************************************************

void loop() {
  getMAC();
  testIMU();
  testLED();
  testMOT();
  testIRL();
  testBUZ();
}
