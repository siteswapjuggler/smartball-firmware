//------------------------------------------
// MACROS, CONSTANTS AND LIBRARIES
//------------------------------------------

#define NUM_LEDS  6
#define LED_CS    D0
#include <Adafruit_DotStar.h>
Adafruit_DotStar strip = Adafruit_DotStar(NUM_LEDS,DOTSTAR_BGR);

#define MOT_PIN   D2
#define IRL_PIN   D1

#include <SB_MPU9250.h>
MPU9250 IMU(SPI, D3);

#define SPEED    500
#define VALUE    420

//******************************************************************************************

void setup() {
  Serial.begin(115200);
  initIMU();
  initLED();
  initMOT();
  initIRL();
}

//******************************************************************************************

void loop() {
  testIMU();
  testLED();
  testMOT();
  testIRL();
}
