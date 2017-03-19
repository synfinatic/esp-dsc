# ESP-DSC: Digital Setting Circles for ESP-12E/F

## About

Low cost alternative to my original Digital Setting Circles project the
[Teensy-DSC](https://github.com/synfinatic/teensy-dsc/).  This new design
uses the cheap ESP-12E/F WiFi enabled microcontroller which saves about 60% 
of the original price (~$40 vs ~$100).

I'm also planing to include the necessary .STL files so you can 3D print
the appropriate enclosure for the board.

More information about this project is available [on my website](https://synfin.net/sock_stream/tag/esp-dsc).

## Status

 * Design PCB (Initial beta design is complete)
 * Port over the TeensyDSC code to the ESP-12 (WIP)
 * Verify that the ESP-12E is capable of handeling 2.5k encoders (complete!)
 * Verify that the ESP-12E is capable of handeling 10k encoders (TBD)
 * Create the necessary STL files to 3D print an enclosure for the board (TBD)

## Basic Features
I wanted to do my own digital setting circles (DSC) for my Apertura AD12.
The main goals were:

 * Support for at least 40K resolution via quadurature decoding (10k encoders)
 * Built in WiFi access point to allow wirelessly interfacing to an Apple iPad or Android tablet
 * Supporting at least [SkySafari Plus/Pro](http://www.skysafariastronomy.com/)
 * Powered via USB connection
 * Using industry standard RJ45 connection to encoders
 * Open source software & PCB design (GPLv3)
    * PCB Design done in [KiCad](http://www.kicad-pcb.org)
    * PCB's available from [OshPark](http://www.oshpark.com)
 * Inexpensive!  All the components should be available for < $40.

## Programming
You will need the Arduino 1.8.1+ IDE.   You will need to add support for the 
ESP8266 by [following the directions here](https://github.com/esp8266/Arduino).

Before you program your ESP-DSC you will need to:
 * Copy src/esp_dsc/config.h.sample to src/esp_dsc/config.h 
 * edit src/esp_dsc/config.h 
 * edit src/esp_dsc/esp_dsc.h

Under the tools menu, configure your board as follows:
 * Board: NodeMCU 1.0 (ESP-12E Module)
 * Flash Size: 4M (3M SPIFFS)
 * CPU Frequency: 80Mhz
 * Upload Speed: 115200 Baud

Do the following *exactly*:

 1. Disconnect the ESP-DSC from the encoders
 1. Connect a FTDI board to the FTDI header on the ESP-DSC and plug that into your computer
 1. Select the FTDI as your serial port in the Arduino IDE Tools menu
 1. Press Reset button
 1. Press Program button
 1. Release Reset button
 1. Release Program button
 1. Press the Upload button in the Arduino IDE Tools menu

## Supported Software

 * SkySafari Plus/Pro 
    - Select "Basic Encoder System"
    - Encoder Steps: Get Automatically
    - TCP Port 4030

## Similar Projects
There are a bunch of similar projects to ESP-DSC which you should be aware of:
 * [TeensyDSC](https://github.com/synfinatic/teensy-dsc/)
 * [Dave Ek's Setting Circles](http://eksfiles.net/digital-setting-circles/)
 * [mrosseel/arduino-dsc](https://github.com/mrosseel/arduino-dsc)
 * [mdw123/arduino-dsc](https://github.com/mdw123/arduino-dsc)
 * [Arduino Bluetooth DSC](http://orlygoingthirty.blogspot.com/2012/01/arduino-bluetooth-digital-setting.html)
 * [Mike Fulbright's Arduino DSC](http://msfastro.net/articles/arduinodsc/)
