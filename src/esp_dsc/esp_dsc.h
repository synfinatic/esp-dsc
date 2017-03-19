#ifndef __ESP_DSC_H__
#define __ESP_DSC_H__

#define DEBUG 1

/* Encoder pins on RJ45 */
#define CHAN_RA_A   2     // RJ45 pin 3
#define CHAN_RA_B  15     // RJ45 pin 1
#define CHAN_DEC_A  5     // RJ45 pin 7
#define CHAN_DEC_B  4     // RJ45 pin 5

#define ESP_DSC_VERSION "0.0.1"
#define MAX_SRV_CLIENTS 4
#define MAX_AP_RETRIES 60

// Resolution of encoders after quadrature decoding
// must be a float!
#define RA_RESOLUTION 10000.0
#define DEC_RESOLUTION 10000.0

#define SERIAL_SPEED 115200 // 9600
#define SERIAL_PRINT_DELAY 1000

#define AP_MODE 1 // Comment out this to connect to an existing WiFi network

/* 
 * For security reasons, define your WIFI_SSID and WIFI_PASSWORD 
 * in password.h
 */
#include "passwords.h"

#define TCP_PORT 4030
#define BUFF_LEN 10

#endif // __ESP_DSC_H__
