#ifndef SMARTBALL_h
#define SMARTBALL_h

//---------------------------------------------------------------
// COLOR CONSTANT
//---------------------------------------------------------------

#define RED    0xFF0000
#define BLUE   0x0000FF
#define GREEN  0x00FF00
#define BLACK  0x000000

#define WAIT   0X8F4E00
#define ALERT  0X7F0000

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
#define CMD_GENERAL  0x12 
#define SAVE_GENERAL 0x13
#define CMD_COLOR1 	 0x20
#define CMD_STREAM   0x21
#define CMD_COLOR2   0x22
#define CMD_IMU      0x30
#define CMD_ACCRANGE 0x32
#define CMD_GYRRANGE 0x33
#define SAVE_IMU     0x31
#define CMD_IRL      0x40
#define CMD_MOT      0x50
#define CMD_STB		 0x60
#define CMD_MST		 0x70

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
// EEPROM ADDRESSES
//---------------------------------------------------------------

#define FS_ADDR  0x00
#define IS_ADDR  0x10
#define GS_ADDR  0x20
#define WS_ADDR  0x40

//---------------------------------------------------------------
// DATA STRUCTURES
//---------------------------------------------------------------

#define IP_LEN 16
#define PWD_LEN  64
#define SSID_LEN 64

struct _factorySettings {
  uint16_t serialNumber;
  uint16_t deviceFlag;
  float adcCalibration;
};

struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte gyrRange;
};

struct _generalSettings {
  uint16_t idNumber;
};

struct _wifiSettings {
  char ssid[SSID_LEN];
  char password[PWD_LEN];
  char outputIp[IP_LEN];
};

#endif