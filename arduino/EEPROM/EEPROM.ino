/* -------------------------------------------------------------------------------------
   THE SMARTBALL PROJECT - 08/09/2019
   Copyright 2013-2022 Sylvain GARNAVAULT
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
   IDE Configuration

   - LOLIN(WEMOS) D1 R2 & Mini
   - 4M (2M SPIFFS)
   - Disabled
   - Nothing
   - v2 Lower Memory
   - Flash
   - 160 MHz
   - 921600
   - Only Sketch
*/

//add no Wifi to this programm

#include <EEPROM.h>
#include <ESP8266WiFi.h>

//----------------------------------------------------------------------------------
// USER PARAMETERS
//----------------------------------------------------------------------------------

#define SERIAL_NUMBER   38
#define ID_NUMBER       1
#define DEVICE_FLAG     0b1111
#define DEFAULT_SSID    "network"
#define DEFAULT_PWD     "password"
#define DEFAULT_IP      "192.168.1.10"
#define DEFAULT_STATIC  "192.168.1.100"
#define DEFAULT_GATEWAY "192.168.1.1"
#define DEFAULT_MASK    "255.255.255.0"

//----------------------------------------------------------------------------------
// EEPROM VERSION
//----------------------------------------------------------------------------------

#define MAJOR    0
#define MINOR    5
#define REVISION 0

//#define RESET_PARAMETERS

//----------------------------------------------------------------------------------
// SET & READ EEPROM
//----------------------------------------------------------------------------------

String eepromVersion;

void setup() {
  Serial.begin(115200);

#ifdef RESET_PARAMETERS
  Serial.println("Set & save default parameters");
  setDefaultParameters();
  saveDefaultParameters();
#endif

  Serial.println("Get new parameters from EEPROM\n");
  getParameters();
  eepromVersion = "v" + String(MAJOR) + "." + String(MINOR) + "." + String(REVISION);
}

//----------------------------------------------------------------------------------
// PRINT EEPROM PARAMETERS
//----------------------------------------------------------------------------------

void loop() {
  Serial.println("-------------------------------------------");
  Serial.println("Print EEPROM parameters / " + eepromVersion);
  Serial.println("-------------------------------------------");
  printParameters();
  Serial.println();
  delay(1000);
}
