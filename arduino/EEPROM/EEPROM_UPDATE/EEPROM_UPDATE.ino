//---------------------------------------------------------------
// OLD EEPROM
//---------------------------------------------------------------

#include <EEPROM.h>

#define OLD_FACTORY_SETTINGS_ADDR   0x00
#define OLD_IMU_SETTINGS_ADDR       0x10

struct _oldFactorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _oldImuSettings {
  byte streamFlag;
  byte accRange;
  byte accSmoothing;
  byte gyrRange;
  byte gyrSmoothing;
  byte magSmoothing;
};

_oldFactorySettings ofset;
_oldImuSettings     oiset;

//-----------------------------------------------------------------------------------
// NEW EEPROM
//-----------------------------------------------------------------------------------

#define BOARD_VERSION 1
#include "header/smartball.h"

_factorySettings fset;
_imuSettings     iset;
_generalSettings gset;
_wifiSettings    wset;

//---------------------------------------------------------------
// LED FEEDBACK
//---------------------------------------------------------------

#include <Adafruit_DotStar.h>
Adafruit_DotStar strip = Adafruit_DotStar(RGB_NUM, D7, D5, DOTSTAR_BGR);

void setup() {
  initRGB();
  EEPROM.begin(512);
  EEPROM.get(OLD_FACTORY_SETTINGS_ADDR, ofset);
  EEPROM.get(OLD_IMU_SETTINGS_ADDR, oiset);
  EEPROM.end();
  changeRGB(0x800000);
  delay(1000);

  fset.serialNumber = ofset.serialNumber;
  fset.deviceFlag = 3;
  fset.adcCalibration = 1000.;
  iset.streamFlag = oiset.streamFlag;
  iset.accRange = 3;
  iset.gyrRange = 3;
  gset.idNumber = ofset.serialNumber;
  strcpy(wset.ssid, "Les Objets Volants");
  strcpy(wset.password, "lesobjetsvolants");
  strcpy(wset.outputIp, "239.0.0.51");

  EEPROM.begin(512);
  EEPROM.put(FS_ADDR, fset);
  EEPROM.put(IS_ADDR, iset);
  EEPROM.put(GS_ADDR, gset);
  EEPROM.put(WS_ADDR, wset);
  EEPROM.end();

  changeRGB(0x000080);
  delay(1000);
}

void loop() {
  changeRGB(0x008000);
  delay(500);
  changeRGB(0x000000);
  delay(500);
}

//---------------------------------------------------------------
// LEDS FUNCTIONS
//---------------------------------------------------------------

void initRGB() {
  pinMode(RGB_CS, OUTPUT);
  digitalWrite(RGB_CS, LOW);
  strip.begin();
  digitalWrite(RGB_CS, HIGH);
}

void changeRGB(int32_t c) {
  for (int i = 0; i < RGB_NUM; i++)
    strip.setPixelColor(i, c);
  updateRGB();
}

void updateRGB() {
  digitalWrite(RGB_CS, LOW);
  strip.show();
  digitalWrite(RGB_CS, HIGH);
}
