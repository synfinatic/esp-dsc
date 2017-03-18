# ESP-DSC: Digital Setting Circles for ESP-12E/F

## About

Low cost alternative to my original Digital Setting Circles project the
[Teensy-DSC](https://github.com/synfinatic/teensy-dsc/).  This new design
uses the cheap ESP-12E/F WiFi enabled microcontroller which saves about 60% 
of the original price (~$40 vs ~$100).

I'm also planing to include the necessary .STL files so you can 3D print
the appropriate enclosure for the board.

## Status

 * Design PCB (Initial beta design is complete)
 * Port over the TeensyDSC code to the ESP-12 (TBD)
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

Under the tools menu, configure your board as follows:
 * Board: NodeMCU 1.0 (ESP-12E Module)
 * Flash Mode: DIO
 * Flash Size: 4M (3M SPIFFS)
 * CPU Frequency: 80Mhz
 * Upload Speed: 115200 Baud

## Similar Projects
There are a bunch of similar projects to TeensyDSC which you should be aware of:
 * [TeensyDSC](https://github.com/synfinatic/teensy-dsc/)
 * [Dave Ek's Setting Circles](http://eksfiles.net/digital-setting-circles/)
 * [mrosseel/arduino-dsc](https://github.com/mrosseel/arduino-dsc)
 * [mdw123/arduino-dsc](https://github.com/mdw123/arduino-dsc)
 * [Arduino Bluetooth DSC](http://orlygoingthirty.blogspot.com/2012/01/arduino-bluetooth-digital-setting.html)
 * [Mike Fulbright's Arduino DSC](http://msfastro.net/articles/arduinodsc/)

## Similar Commercial Products
There are also a number of commercial products which share many of the same features of TeensyDSC:

 * [Southern Stars/Orion SkyFi](http://www.southernstars.com/products/skyfi/)
 * [Astro Devices Nexus](http://www.astrodevices.com/products/Nexus/Nexus.html)
 * [Astro Devices Nexus-S](http://www.astrodevices.com/products/NexusS/NexusS.html)

