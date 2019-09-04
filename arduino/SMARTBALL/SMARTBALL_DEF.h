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

#define CMD_BAT       0x00		
#define CMD_PING      0x01
#define CMD_REBOOT    0x02
#define CMD_FACTORY   0x10
#define SAVE_FACTORY  0x11
#define CMD_GENERAL   0x12 
#define SAVE_GENERAL  0x13
#define CMD_COLOR1 	  0x20
#define CMD_STREAM    0x21
#define CMD_COLOR2    0x22
#define CMD_IMU       0x30
#define SAVE_IMU      0x31
#define CMD_ACCRANGE  0x32
#define CMD_GYRRANGE  0x33
#define CMD_IRL       0x40
#define CMD_MOT       0x50
#define CMD_STB		    0x60
#define CMD_MST		    0x70

//---------------------------------------------------------------
// ESP8266 PINOUT
//---------------------------------------------------------------

#if !defined(BOARD_VERSION) || BOARD_VERSION == 1
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
// IMU FLAG ADDRESSES (8bit)
//---------------------------------------------------------------

#define ACC_BIT  0  // Accelerometer
#define GYR_BIT  1  // Gyroscope
#define MAG_BIT  2  // Magnetometer
#define TMP_BIT  3  // Temperature
#define VEC_BIT  4  // IMU Magnitude
#define QUA_BIT  5  // Orientation Quaternion
#define WLD_BIT  6  // World Acceleration  
#define STA_BIT	 7  // State Detection

//---------------------------------------------------------------
// DEVICE FLAG ADDRESSES (8bit)
//---------------------------------------------------------------

#define RGB_BIT  0
#define IMU_BIT  1
#define IRL_BIT  2
#define MOT_BIT  3
#define BUZ_BIT  4

//---------------------------------------------------------------
// STREAM FLAG ADDRESSES (16bit)
//---------------------------------------------------------------

#define COL_STREAM_MASK   0b111
#define COL1_STREAM_ADDR  0
#define COL2_STREAM_ADDR  3
#define MST_STREAM_BIT    6
#define STB_STREAM_BIT    7
#define IRL_STREAM_BIT    8
#define MOT_STREAM_BIT    9
#define LOOP_STREAM_BIT	  15

//---------------------------------------------------------------
// EEPROM ADDRESSES
//---------------------------------------------------------------

#define FS_ADDR  0x00
#define IS_ADDR  0x10
#define GS_ADDR  0x20
#define WS_ADDR  0x40

//---------------------------------------------------------------
// EEPROM DATA STRUCTURES
//---------------------------------------------------------------

#define IP_LEN 16
#define PWD_LEN  64
#define SSID_LEN 64

// Memory usage: 10/16 bytes
struct _factorySettings {
  uint16_t serialNumber;    // unique ID depending on manufacturing
  uint16_t deviceFlag;      // subsystems definition 
  float adcCalibration;     // scale factor for adc
  //uint16_t eepromVersion;   // TODO eeprom versionning
};

// Memory usage: 4/16 bytes
struct _imuSettings {
  byte streamFlag;
  byte accRange;
  byte gyrRange;
  //byte refreshRate;         // TODO 100 Hz vs 50 Hz
};

// Memory usage: 2/16 bytes
struct _generalSettings {
  uint16_t idNumber;
};

// Memory usage: 144 bytes
struct _wifiSettings {
  char ssid[SSID_LEN];
  char password[PWD_LEN];
  char outputIp[IP_LEN];
};

#endif
