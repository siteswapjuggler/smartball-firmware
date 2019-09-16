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
- choosing your IDE options
- upload EEPROM factory settings
- upload the firmware
- upload SPIFFS data

### Installing Arduino IDE 

Just download the Arduino IDE from this website https://www.arduino.cc/en/Main/Software and install it into your computer.

### Installing ESP8266 boards in Arduino IDE

Follow the "Installing with Board Manager" steps from this repository https://github.com/esp8266/Arduino.

### Installing firmware dependencies

This firmware use two Arduino librairies, one for the dotstars driving and the other one for the MPU9250 sensor. Both libraries are joined in the 'arduino/LIBRARIES' subfolder. You can add them within the Arduino IDE by following those instructions: https://www.arduino.cc/en/Guide/Libraries

### Choosing IDE options 

You have now to configure the boards options in the IDE, those are the good option : 
- LOLIN(WEMOS) D1 R2 & Mini
- 4M (2M SPIFFS)
- Disabled
- Nothing
- v2 Lower Memory 
- Flash
- 160 MHz
- 921600
- Only Sketch

### Upload EEPROM factory settings

After configuring the IDE, boards and libraries, just plug your Smartball, choose its serial port in the "Tool" menu and upload the firmware sketch stored in the 'arduino/EEPROM' subfolder. Remember to change to Serial Number according to your ball. Be patient, ESP8266 sketches can takes some time to compile. The communication can fail with high baudrates, check for power on, unplug and replug and try again.

**ATTENTION:** you need to power on the ball before uploading...


### Upload the firmware

After configuring the IDE, boards, libraries and setting up EEPROM factory settings, just plug your Smartball, choose its serial port in the "Tool" menu and upload the firmware sketch stored in the 'arduino/SMARTBALL' subfolder. Be patient, ESP8266 sketches can takes some time to compile. The communication can fail with high baudrates, check for power on, unplug and replug and try again.

**ATTENTION:** you need to power on the ball before uploading...

### Upload SPIFFS data

The new firmware use a SPI File System to store the webserver data files. You can upload those data by installing the file system upload as described in the repository : https://github.com/esp8266/arduino-esp8266fs-plugin, then using the *Tools > ESP8266 Sketch Data Upload* menu item within the Arduino IDE with the firmware sketch opened.

**ATTENTION:** you need to power on the ball before uploading...

## Linked repositories

This repository refers directly to its following siblings : 

- https://github.com/siteswapjuggler/smartball-externals
- https://github.com/siteswapjuggler/smartball-hardware
- https://github.com/siteswapjuggler/smartball-documents
- https://github.com/siteswapjuggler/smartball-imu
