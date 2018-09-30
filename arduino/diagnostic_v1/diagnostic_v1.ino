
#define NUM_LEDS  6
#define LED_CS    D0

#include <Adafruit_DotStar.h>
Adafruit_DotStar strip = Adafruit_DotStar(NUM_LEDS,DOTSTAR_BGR);

#include <MPU9250.h>
MPU9250 IMU(SPI, D3);

void setup() {
  Serial.begin(115200);
  IMU.begin();

  strip.begin();
  pinMode(LED_CS, OUTPUT);
  digitalWrite(LED_CS, HIGH);
  changeAll(0x0);
}

void loop() {
  changeAll(0xFF0000);
  delay(500);
  changeAll(0x00FF00);
  delay(500);
  changeAll(0x0000FF);
  delay(500);
  changeAll(0xFFFFFF);
  delay(500);

  IMU.readSensor();
  // display the data
  Serial.print(IMU.getAccelX_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(), 2);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(), 2);
  Serial.print("\t");
  Serial.println(IMU.getTemperature_C(), 2);
}

void changeAll(int32_t color) {
  for (int i = 0; i < 6; i++) {
    strip.setPixelColor(i, color);
  }
  strip.updatePins(D7, D5);
  digitalWrite(LED_CS, LOW);
  strip.show();
  digitalWrite(LED_CS, HIGH);
  strip.updatePins();
}
