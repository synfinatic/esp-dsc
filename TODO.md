# v2.0.0 board:
 - Move RA-ChanA off pin 2 which conflicts with the onboard LED
 - Fix label on Reset Switch
 - Make board smaller
 - Improve programming reliability
    - https://blog.falafel.com/how-to-wire-the-esp8266-for-programming/
    - https://github.com/esp8266/Arduino/blob/master/doc/ESP_to_serial.png
 - Fix RJ45 mounting holes
 - Remove option of running power off FTDI board (ESP will crash on boot)
 - External pull up resitors for encoders?
 - May need to use LSI 7355R-S for encoders?
    - Provide SPI interface to avoid interrupts/polling.  
    - Need one per encoder ($4-6/ea)
    - Hard to find! (not Digikey/Newark/etc)
    - http://www.usdigital.com/products/interfaces/ics/LFLS7366R-S
    - http://www.omnipro.net/lsi-csi/LS7366R-S

# Software
 - Fix interrupt handler to not miss clicks
 - use WiFiManager library
 - Save encoder resolution to flash
 - Investigate webserver for config
 - Display MAC address for DHCP hard coding
 - Improve use of DEBUG
