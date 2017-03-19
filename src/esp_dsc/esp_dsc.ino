/*
 * ESP-12E/F Digital Setting Circles
 * Copyright (C) 2017 Aaron Turner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Quadrature decoding based upon:
 * http://yameb.blogspot.com/2012/11/quadrature-encoders-in-arduino-done.html
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "esp_dsc.h"
#include "dsc.h"
#include "utils.h"

void HandleChanRA();
void HandleChanDEC();
int ParseEncoderRA();
int ParseEncoderDEC();

WiFiServer server(TCP_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void
setup() {
    uint8_t i = 0;
    Serial.begin(SERIAL_SPEED);

    pinMode(CHAN_RA_A, INPUT_PULLUP);
    pinMode(CHAN_RA_B, INPUT_PULLUP);
    pinMode(CHAN_DEC_A, INPUT_PULLUP);
    pinMode(CHAN_DEC_B, INPUT_PULLUP);

#ifdef AP_MODE
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED && i++ < MAX_AP_RETRIES) {
        Serial.print("Waiting for WiFi status to go live...\n");
        delay(500);
    }
    if (i > MAX_AP_RETRIES) {
        Serial.print("Could not connect to ");
        Serial.println(WIFI_AP);
        while(1)
            delay(500);
    }
#endif

    server.begin();
    server.setNoDelay(true);
    Serial.print("Connect to ");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.println(TCP_PORT);

    attachInterrupt(CHAN_RA_A, HandleChanRA, CHANGE);
    attachInterrupt(CHAN_RA_B, HandleChanRA, CHANGE);
    attachInterrupt(CHAN_DEC_A, HandleChanDEC, CHANGE);
    attachInterrupt(CHAN_DEC_B, HandleChanDEC, CHANGE);
}



// Quadrature encoders
// Left encoder
#define EncoderIsReversed

volatile bool _RAEncoderASet;
volatile bool _RAEncoderBSet;
volatile bool _RAEncoderAPrev;
volatile bool _RAEncoderBPrev;
volatile long _RAEncoderTicks = 0;

volatile bool _DECEncoderASet;
volatile bool _DECEncoderBSet;
volatile bool _DECEncoderAPrev;
volatile bool _DECEncoderBPrev;
volatile long _DECEncoderTicks = 0;

void
process_client(uint8_t c) {
    char buff[36];
    char *value;
    char val = serverClients[c].read();
    if (val == 'Q') {
        value = EncoderValue(
                ngc_convert_encoder_value(_RAEncoderTicks, RA_RESOLUTION), true);
        sprintf(buff, "%s\t", value);
        value = EncoderValue(
                ngc_convert_encoder_value(_DECEncoderTicks, DEC_RESOLUTION), true);
        strcat(buff, value);
        strcat(buff, "\r\n");
        serverClients[c].print(buff);
        Serial.print("Received query from ");
        Serial.println(c);
    } else {
        Serial.print("Unknown request: ");
        Serial.println(val);
    }
}

void 
loop()
{ 
    static unsigned long last = 0;
    unsigned long now = millis();
    uint8_t c;

    // look for new clients
    if (server.hasClient()) {
        for (c = 0; c < MAX_SRV_CLIENTS; c++) {
            // find a free slot
            if (!serverClients[c] || !serverClients[c].connected()) {
                if (serverClients[c]) 
                    serverClients[c].stop();
                serverClients[c] = server.available();
                continue;
            }
        }
        
        // no free slots to reject
        WiFiClient serverClient = server.available();
        serverClient.stop();
    }

    // check clients for data
    for (c = 0; c < MAX_SRV_CLIENTS; c++) {
        if (serverClients[c] && serverClients[c].connected()) {
            if (serverClients[c].available()) {
                process_client(c);
            }
        }
    }

    if ((now - last) >= SERIAL_PRINT_DELAY) {
        last = now;

        Serial.print("RA Encoder Ticks: ");
        Serial.print(ngc_convert_encoder_value(_RAEncoderTicks, RA_RESOLUTION));
        Serial.print("  Revolutions: ");
        Serial.print(_RAEncoderTicks/RA_RESOLUTION);
        Serial.print("\n");
        Serial.print("DEC Encoder Ticks: ");
        Serial.print(ngc_convert_encoder_value(_DECEncoderTicks, DEC_RESOLUTION));
        Serial.print("  Revolutions: ");
        Serial.print(_DECEncoderTicks/DEC_RESOLUTION);
        Serial.print("\n");
    }
}


// Interrupt service routines for the left motor's quadrature encoder
void 
HandleChanRA() {
    _RAEncoderASet = digitalRead(CHAN_RA_A);
    _RAEncoderBSet = digitalRead(CHAN_RA_B);

    _RAEncoderTicks += ParseEncoderRA();

    _RAEncoderAPrev = _RAEncoderASet;
    _RAEncoderBPrev = _RAEncoderBSet;
}

// Interrupt service routines for the right motor's quadrature encoder
void 
HandleChanDEC() {
    // Test transition;
    _DECEncoderASet = digitalRead(CHAN_DEC_A);
    _DECEncoderBSet = digitalRead(CHAN_DEC_B);

    _DECEncoderTicks += ParseEncoderDEC();

    _DECEncoderAPrev = _DECEncoderASet;
    _DECEncoderBPrev = _DECEncoderBSet;
}

int 
ParseEncoderRA() {
    if (_RAEncoderAPrev && _RAEncoderBPrev) {
        if (!_RAEncoderASet && _RAEncoderBSet) return 1;
        if (_RAEncoderASet && !_RAEncoderBSet) return -1;
    } else if (!_RAEncoderAPrev && _RAEncoderBPrev) {
        if (!_RAEncoderASet && !_RAEncoderBSet) return 1;
        if (_RAEncoderASet && _RAEncoderBSet) return -1;
    } else if (!_RAEncoderAPrev && !_RAEncoderBPrev) {
        if (_RAEncoderASet && !_RAEncoderBSet) return 1;
        if (!_RAEncoderASet && _RAEncoderBSet) return -1;
    } else if (_RAEncoderAPrev && !_RAEncoderBPrev) {
        if (_RAEncoderASet && _RAEncoderBSet) return 1;
        if (!_RAEncoderASet && !_RAEncoderBSet) return -1;
    }
}

int 
ParseEncoderDEC() {
    if (_DECEncoderAPrev && _DECEncoderBPrev) {
        if (!_DECEncoderASet && _DECEncoderBSet) return 1;
        if (_DECEncoderASet && !_DECEncoderBSet) return -1;
    } else if (!_DECEncoderAPrev && _DECEncoderBPrev) {
        if (!_DECEncoderASet && !_DECEncoderBSet) return 1;
        if (_DECEncoderASet && _DECEncoderBSet) return -1;
    } else if (!_DECEncoderAPrev && !_DECEncoderBPrev) {
        if (_DECEncoderASet && !_DECEncoderBSet) return 1;
        if (!_DECEncoderASet && _DECEncoderBSet) return -1;
    } else if (_DECEncoderAPrev && !_DECEncoderBPrev) {
        if (_DECEncoderASet && _DECEncoderBSet) return 1;
        if (!_DECEncoderASet && !_DECEncoderBSet) return -1;
    }
}
