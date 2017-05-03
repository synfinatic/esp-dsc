#ifndef __ESP_DSC_H__
#define __ESP_DSC_H__

#include <Arduino.h>

/* 
 * For security reasons, define your WIFI_SSID and WIFI_PASSWORD 
 * in config.h
 */
#include "config.h"

/*
 * DO NOT EDIT BEYOND THIS POINT!
 */

/* Encoder pins on RJ45 */
#define CHAN_DEC_A   5     // RJ45 pin 7
#define CHAN_DEC_B   4     // RJ45 pin 5
#define CHAN_RA_A    2     // RJ45 pin 3
#define CHAN_RA_B   15     // RJ45 pin 1

#define ESP_DSC_VERSION "0.0.2"
#define MAX_SRV_CLIENTS 4
#define MAX_AP_RETRIES 60
#define SERIAL_SPEED 115200
#define SERIAL_PRINT_DELAY 1000

#define BUFF_LEN 10
#define CLIENT_BUFF_LEN 36

#endif // __ESP_DSC_H__
