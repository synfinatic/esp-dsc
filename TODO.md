# v2.0.0 board:
 - Move RA-ChanA off pin 2 which conflicts with the onboard LED
 - Fix label on Reset Switch
 - Make board smaller
 - Improve programming reliability
    - https://blog.falafel.com/how-to-wire-the-esp8266-for-programming/
    - https://github.com/esp8266/Arduino/blob/master/doc/ESP_to_serial.png
 - Fix RJ45 mounting holes
 - Remove option of running power off FTDI board (ESP will crash on boot)

# Software
 - Save encoder resolution to flash?
 - Investigate webserver for config
 - Display MAC address for DHCP hard coding
 - Improve use of DEBUG
