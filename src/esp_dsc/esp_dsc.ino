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
 */

#include <Arduino.h>
#include "esp_dsc.h"
#include "dsc.h"
#include "utils.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
Encoder EncoderRA(CHAN_RA_A, CHAN_RA_B);
Encoder EncoderDEC(CHAN_DEC_A, CHAN_DEC_B);

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiServer server(TCP_PORT);
WiFiClient serverClients[MAX_SRV_CLIENTS];
void process_client(uint8_t c);

long RA_Res = RA_RESOLUTION;
long DEC_Res = DEC_RESOLUTION;


void
setup() {
    uint8_t i = 0;
    Serial.begin(SERIAL_SPEED);

#ifdef DEBUG
    pinMode(0, INPUT_PULLUP);
#endif

#ifdef ENABLE_WIFI
    Serial.println("Starting up WiFi...");
#ifdef AP_MODE
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Waiting for WiFi status to go live...\n");
        delay(500);
    }
    Serial.printf("Connected to %s!\n", WIFI_AP);
#endif
    server.begin();
    server.setNoDelay(true);
//     Serial.printf("Connect to %s:%d\n", WiFi.localIP(), TCP_PORT);
#else
    Serial.println("WiFi disabled!");
#endif // ENABLE_WIFI

}


#ifdef ENABLE_WIFI
/*
 * Process TCP client request on the given serverClient entry
 */
void
process_client(uint8_t c) {
    char buff[CLIENT_BUFF_LEN];
    char *value;
    char val = serverClients[c].read();
    long ra_value, dec_value;;
    int i, j;
    switch (val) {
        // get encoder values
        case 'Q':
            ra_value = EncoderRA.read();
            dec_value = EncoderDEC.read();
            value = EncoderValue(ngc_convert_encoder_value(ra_value, RA_Res), true);
            sprintf(buff, "%s\t", value);
            value = EncoderValue(
                    ngc_convert_encoder_value(dec_value, DEC_Res), true);
            serverClients[c].printf("%s%s\r\n", buff, value);
            break;
        case 'R':
            // set resolution
            memset(buff, '\0', CLIENT_BUFF_LEN);
            i = 0;
            j = 0;
            // Horrible hack around ESP8266 not having sscanf()
            if (serverClients[c].available()) {
                serverClients[c].read(); // read the space
            } else {
                Serial.printf("short command!\n");
                return;
            }

            while (i < 2) {
                while (serverClients[c].available() && serverClients[c].peek() != ' ') {
                    if (serverClients[c].peek() != ' ') {
                        buff[j] = serverClients[c].read();
                        j++;
                    }
                }
                if (i == 0) {
                    j = 0;
                    ra_value = String(buff).toInt();
                    Serial.printf("RA = %ld\n", ra_value);
                    memset(buff, '\0', CLIENT_BUFF_LEN);
                    serverClients[c].read(); // read the space
                } else {
                    dec_value = String(buff).toInt();
                    Serial.printf("DEC = %ld\n", dec_value);
                }
                i++;
            }
            if (i != 2) {
                Serial.printf("Unable to process: R %s\n", buff);
                return;
            }
            RA_Res = ra_value;
            DEC_Res = dec_value;
            break;
        case 'G':
        case 'H':
            // get resolution
            long ra, dec;
            char ra_pos[2], dec_pos[2];

            ra = abs(RA_Res);
            dec = abs(DEC_Res);

            ra_pos[1] = dec_pos[1] = '\0';
            ra_pos[0] = ra == RA_Res ? '+' : '-';
            dec_pos[0] = dec == DEC_Res ? '+' : '-';

            Serial.printf("%s%05ld\t%s%05ld\r\n", ra_pos, RA_Res, dec_pos, dec); 
            serverClients[c].printf("%s%05ld\t%s%05ld\r\n", ra_pos, RA_Res, dec_pos, dec); 
            break; 
        case 'V':
        case 'v':
            // get version
            serverClients[c].printf("ESP-DSC v%s\n", ESP_DSC_VERSION);
            break;
        default:
            Serial.printf("Unknown command: %c\n", val);
    }
}
#endif // ENABLE_WIFI


void 
loop()
{ 
    static unsigned long last = 0;
    unsigned long now = millis();
    uint8_t c;
    long dec_value, ra_value;
    char buffer[50], ra_f[10], dec_f[10];
#ifdef ENABLE_WIFI
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
#endif // ENABLE_WIFI

#ifdef DEBUG
    if ((now - last) >= SERIAL_PRINT_DELAY) {
        last = now;
        if (LOW == digitalRead(0)) {
            // zero out encoder values for testing
            noInterrupts();
            EncoderRA.write(0);
            EncoderDEC.write(0);
            interrupts();
        }
        ra_value = EncoderRA.read();
        dec_value = EncoderDEC.read();
        snprintf(buffer, 100,
                "RA Ticks: %ld\tRevs: %s\nDec Ticks: %ld\tRevs: %s\n",
                ngc_convert_encoder_value(ra_value, RA_Res),
                ftoa(ra_f, ra_value/(float)RA_Res, 3),
                ngc_convert_encoder_value(dec_value, DEC_Res),
                ftoa(dec_f, dec_value/(float)DEC_Res, 3));
        Serial.print(buffer);
    }
#endif
    delay(25); // service WiFiClient
}
