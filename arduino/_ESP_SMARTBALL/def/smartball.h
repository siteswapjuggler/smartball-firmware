//---------------------------------------------------------------
// COLOR CONSTANT
//---------------------------------------------------------------

#define RED   0xFF0000
#define BLUE  0x0000FF
#define GREEN 0x00FF00
#define BLACK 0x000000

//---------------------------------------------------------------
// EEPROM ADDRESSES
//---------------------------------------------------------------

#define FACTORY_SETTINGS_ADDR   0x00
#define IMU_SETTINGS_ADDR       0x10

//---------------------------------------------------------------
// DATAGRAM PROTOCOL
//---------------------------------------------------------------

#define SOP          0xE7		// packet header

#define CMD_BAT      0x00		
#define CMD_PING     0x01
#define CMD_REBOOT   0x02
#define CMD_SLEEP    0x03
#define CMD_FACTORY  0x10
#define SAVE_FACTORY 0x11
#define CMD_COLOR    0x20
#define CMD_STREAM   0x21
#define CMD_IMU      0x30
#define CMD_ACCRANGE 0x32
#define CMD_GYRRANGE 0x33
#define SAVE_IMU     0x31
#define CMD_IRL      0x40
#define CMD_MOT      0x50

//---------------------------------------------------------------
// ESP8266 PINOUT
//---------------------------------------------------------------

#ifndef BOARD_VERSION
#define BATT_PIN  A0
#define RGB_CS	  D0
#define IMU_CS    D3
#define IRL_PIN   D1
#define MOT_PIN   D2

#elif BOARD_VERSION == 1
#define BATT_PIN  A0
#define RGB_CS	  D0
#define IMU_CS    D3
#define IRL_PIN   D1
#define MOT_PIN   D2
#endif

//---------------------------------------------------------------
// LED BOARD OPTIONS
//---------------------------------------------------------------

#define RGB_NUM   6

//---------------------------------------------------------------
// IMU FLAG ADDRESSES
//---------------------------------------------------------------

#define ACC_BIT  0
#define GYR_BIT  1
#define MAG_BIT  2
#define TMP_BIT  3

//---------------------------------------------------------------
// DEVICE FLAG ADDRESSES
//---------------------------------------------------------------

#define RGB_BIT  0
#define IMU_BIT  1
#define IRL_BIT  2
#define MOT_BIT  3
#define BUZ_BIT  4

//---------------------------------------------------------------
// DATA STRUCTURES
//---------------------------------------------------------------

struct _factorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte accSmoothing;
  byte gyrRange;
  byte gyrSmoothing;
  byte magSmoothing;
};

