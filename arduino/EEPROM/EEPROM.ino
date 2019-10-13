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
*/

#include <EEPROM.h>
#include <ESP8266WiFi.h>

//----------------------------------------------------------------------------------
// USER PARAMETERS
//----------------------------------------------------------------------------------

#define ID_NUMBER       5
#define SERIAL_NUMBER   31
#define DEVICE_FLAG     3
#define DEFAULT_SSID    "ssid"
#define DEFAULT_PWD     "password"
#define DEFAULT_IP      "192.168.0.50"
#define DEFAULT_STATIC  "192.168.0.131"
#define DEFAULT_GATEWAY "192.168.0.1"
#define DEFAULT_MASK    "255.255.255.0"

//----------------------------------------------------------------------------------
// EEPROM VERSION
//----------------------------------------------------------------------------------

#define MAJOR    0
#define MINOR    4
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
