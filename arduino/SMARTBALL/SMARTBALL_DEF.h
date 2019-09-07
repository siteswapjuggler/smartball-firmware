#ifndef SMARTBALL_h
#define SMARTBALL_h

//-----------------------------------------------------------------------------------
// LIBRARIES
//-----------------------------------------------------------------------------------

#include <FS.h>                // SPIFFS management
#include <Ticker.h>            // ESP8266 scheduling library for regular event
#include <EEPROM.h>            // 512 byte EEPROM Emulation for ESP8266
#include <ESP8266WiFi.h>       // Standard WiFi Library
#include <DNSServer.h>         // DNS Server management
#include <ESP8266WebServer.h>  // allow WebServer for configuration
#include <WiFiUDP.h>           // UDP Protocol Library
#include <OSCBundle.h>         // OSC Protocol Library
#include <MPU9250.h>           // Custom MPU920 SPI Library - see libraries subfolder
#include <Adafruit_DotStar.h>  // Adafruit_Dotstar Library - https://github.com/adafruit/Adafruit_DotStar >> Issue with SPI Mode has been documented here https://github.com/adafruit/Adafruit_DotStar/isetsues/28

//---------------------------------------------------------------
// OPERATION MODE
//---------------------------------------------------------------

enum _opMode : uint8_t {
  SET = 0,
  RUN = 1
};

//-----------------------------------------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------------------------------------

bool    bli, bat;                 // blink or not
bool    dgm, ben;                 // protocol activation
bool    rgb, mot, imu, irl;       // device subsystem boolean

String  eepromVersion;            // EEPROM version
String  availableNetworks;        // list of networks

Ticker  batTicker;                // battery management ticker
Ticker  imuTicker;                // imu management ticker
Ticker  frameTicker;              // main frame ticker

_opMode operationMode;            // running or config

//---------------------------------------------------------------
// VERSIONNING
//---------------------------------------------------------------

#define FIRMWARE_STATE   "WIP"    // WIP, release, beta
#define FIRMWARE_VERSION "0.3.0"  //
#define PROTOCOL_VERSION "0.4.0"  //

//---------------------------------------------------------------
// COLOR CONSTANT
//---------------------------------------------------------------

#define RED    0xFF0000
#define BLUE   0x0000FF
#define GREEN  0x00FF00
#define BLACK  0x000000

#define WAIT_COLOR  0X8F4E00
#define ALERT_COLOR 0X7F0000

//---------------------------------------------------------------
// BLINKS & TIMEOUTS
//---------------------------------------------------------------

#define WIFI_TIMEOUT 5000  // WiFi connection timeout in ms
#define LONG_BLINK   400    // 
#define MEDIUM_BLINK 200    // 
#define QUICK_BLINK  100    //
#define BLINK_GAP    100    //

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
#define DEFAULT_IMU   0x34
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
// DEVICE FLAG ADDRESSES (8bit)
//---------------------------------------------------------------

#define RGB_BIT  0
#define IMU_BIT  1
#define IRL_BIT  2
#define MOT_BIT  3
#define BUZ_BIT  4

//---------------------------------------------------------------
// CONFIG FLAG ADDRESSES (16bit)
//---------------------------------------------------------------

#define BLI_BIT  0        // Blink leds at start
#define BAT_BIT  1        // Send automatic battery information
#define DGM_BIT  8        // Activate DGM protocol
#define BEN_BIT  9        // Activate BenTo protocol

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
// IMU FLAG ADDRESSES (16bit)
//---------------------------------------------------------------

#define ACC_BIT  0        // Accelerometer
#define GYR_BIT  1        // Gyroscope
#define MAG_BIT  2        // Magnetometer
#define TMP_BIT  3        // Temperature
#define VEC_BIT  4        // IMU Magnitude
#define QUA_BIT  5        // Orientation Quaternion
#define WLD_BIT  6        // World Acceleration  
#define STA_BIT  7        // State Detection
#define BUF_BIT 14        // Buffer activation
#define SPD_BIT 15        // Speed variation 50Hz or 100Hz

//---------------------------------------------------------------
// EEPROM ADDRESSES
//---------------------------------------------------------------

#define ES_ADDR  0x00
#define FS_ADDR  0x10
#define GS_ADDR  0x20
#define DS_ADDR  0x30
#define BS_ADDR  0x50
#define WS_ADDR  0x70

//---------------------------------------------------------------
// EEPROM DATA STRUCTURES
//---------------------------------------------------------------

#define IP_LEN 16
#define PWD_LEN  64
#define SSID_LEN 64

// Memory usage: 6/16 bytes
struct _eepromSettings {
  uint32_t credential;      // must be 0xFDB97531
  uint8_t  major;           // major version number
  uint8_t  minor;           // minor version number
  uint8_t  revision;        // revision version number
};

// Memory usage: 8/16 bytes
struct _factorySettings {
  uint16_t serialNumber;    // unique ID depending on manufacturing
  uint16_t deviceFlag;      // subsystems definition
  float adcCalibration;     // scale factor for adc
};

// Memory usage: 8/16 bytes
struct _generalSettings {
  uint16_t idNumber;
  uint16_t imuFlag;
  uint16_t configFlag;
};

// Memory usage: 6/16 bytes
struct _dgmSettings {
  char outputIp[IP_LEN];
  uint16_t  inputPort;
  uint16_t  outputPort;
};

struct _benSettings {
  char outputIp[IP_LEN];
  uint16_t benInputPort;
  uint16_t oscInputPort;
  uint16_t oscOutputPort;
};

// Memory usage: 144 bytes
struct _wifiSettings {
  char ssid[SSID_LEN];
  char password[PWD_LEN];
};

#endif
