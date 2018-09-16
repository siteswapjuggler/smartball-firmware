# Smartball project - firmware

This repository contains the Smartball project firmware.

## Open Source

All the technological development work made in the Smartball project is available under open source licenses. As we don't know yet about how we will deal with further development all of our hardwares will be published under Creative Commons BY-NC-SA and all of our softwares will be under GPL v3.

|Applying to|Licence|Logo|
|---|---|---|
|hardware|[Creatives Commons BY-NC-SA](http://creativecommons.org/licenses/by-nc-sa/4.0/)|[![Creative Commons BY-SA](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png) ](http://creativecommons.org/licenses/by-nc-sa/4.0/)|
|software|[GPL v3](http://www.gnu.org/licenses/gpl.html)|[![GPL V3](https://www.gnu.org/graphics/gplv3-88x31.png)](http://www.gnu.org/licenses/gpl.html)|

## Installation

The Smartball project is an ESP8266 Arduino compatible firmware. In order to upload it to the balls you'll need the following steps. 

- installing Arduino IDE
- installing ESP8266 boards in the Arduino IDE
- installing firmware dependencies
- configuring your firmware
- choosing your IDE options
- upload the firmware

### Installing Arduino IDE 

Just download the Arduino IDE from this website https://www.arduino.cc/en/Main/Software and install it into your computer.

### Installing ESP8266 boards in Arduino IDE

Follow the "Installing with Board Manager" steps from this repository https://github.com/esp8266/Arduino.

### Installing firmware dependencies

This firmware use two Arduino librairies, one for the dotstars driving and the other one for the MPU9250 sensor. Both libraries are joined in the 'arduino/libraries' subfolder. You can add them within the Arduino IDE by following those instructions: https://www.arduino.cc/en/Guide/Libraries

### Configuring your firmware

Before uploading to the balls you need to precise your network password and SSID. This can ben done on line 29 and 30 of the main arduino sketch '_ESP_SMARTBALL.ino'.

You can also force multicast feedback into unicast by replacing outIp address on line 32 by your own computer IP address.

### Choosing IDE options 

You have now to configure the boards options in the IDE, those are the good option : 
- LOLIN(WEMOS) D1 R2 & Mini
- 4M (1M SPIFFS)
- Disabled
- Nothing
- v1.4 Higher Bandwidth
- Flash
- 160 MHz
- 921600
- Only Sketch

### Upload the firmware

After configuring the IDE, boards and libraries, just plug your Smartball, choose its serial port in the "Tool" menu and upload the sketch. ESP8266 sketches takes time to compile so be patient. It appears that sometimes the communication will fail, check for power on, unplug and replug and try again.

**ATENTION:** you need to power on the ball before uploading...


