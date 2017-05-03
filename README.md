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
 * Port over the TeensyDSC code to the ESP-12 (80% done)
 * Verify that the ESP-12E is capable of handeling 2.5k encoders (complete!)
 * Verify that the ESP-12E is capable of handeling 10k encoders (failed!)
 * Redesign PCB for better performance  (TBD)
 * Update code based on new design (TBD)
 * Create the necessary STL files to 3D print an enclosure for the board (TBD)

## Performance

So a quick word about performance.  There are a number of people who claim that 
the ESP-12E has enough horsepower for two 2500PPR (10K after quadurature decoding) 
encoders.  One person told me they saw some errors, but they were small and 
likely caused by their mount.  This didn't seem very scientific to me, so I ran 
some tests.

My methodolgy is pretty simple.  I connected two ESP12E's to the same set of 
encoders.  Both ESP12E's used the same code and a custom modified version of 
[Paul Stoffgren's Encoder library](https://github.com/PaulStoffregen/Encoder) 
which has support for the ESP8266.  I also glued a toothpick to each encoder 
shaft to make it easier to rotate with my finger.  The test would be simple: If
both ESP12E's reported the same values, then I would say it was accurate.  But 
if the results were different, then it wasn't accurate.

I found that I could rotate a single 2,500PPR as fast as I possibly could 
without any error.  I could rotate a single 10,000PPR (40K after quadurature 
decoding) almost as fast without causing any error.

However, once I introduced a second encoder, I started seeing errors.  At about 
1 rotation/second, the 2500PPR encoders started to show some errors.  Two 
10,000PPR encoders had significant errors.  I also noticed that "flicking" the 
encoder causing it to quickly accellerate for a short duration (approx 1/4 
to 1/2 rotation) would cause significant errors even with the 2500PPR encoders.

This indicates that a person who does not very quickly move their scope 
should be able to use two 2500PPR encoders without introducing any 
errors.  If you're using a GoTo mount or a large scope you should be very 
safe.

I've done some testing on my AD12 (12" dobsonian) and 2500PPR/10,000 "count" 
encoders and found accuracy wasn't a problem at all.  The amount of error I saw
was approximagely +/-0.25deg meaning that if you have an EP with a 0.60deg 
effective FOV or more you should be able to use the ESP-DSC to point at an 
object and have it located inside your view through the scope.  Considering 
this is a mass produced dobsonian with a particile board mount, I consider 
this effectively within the error of the mount itself.

For comparison, my 30deg/82FOV Explore Scientific EP has an effective FOV of 
1.62deg in my scope which is more then twice the minimum.  

However I don't feel that it would be reliably accurate using 10,000PPR 
(40K count) encoders.  The result is I don't feel my initial PCB design is 
good enough with a single ESP12E.  I'm currently researching other solutions 
which could be implemented as cheaply and reliably as possible.

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

Under the tools menu, configure your board as follows:
 * Board: NodeMCU 1.0 (ESP-12E Module)
 * Flash Size: 4M (3M SPIFFS)
 * CPU Frequency: 80Mhz
 * Upload Speed: 115200 Baud

Do the following *exactly*:

 1. Disconnect the ESP-DSC from the encoders
 1. Connect a FTDI board to the FTDI header on the ESP-DSC and plug that into your computer
 1. Select the FTDI as your serial port in the Arduino IDE Tools menu
 1. Press and hold the Reset button
 1. Press and hold the Program button
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
