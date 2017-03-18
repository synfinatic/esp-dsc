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
// #include <Flash.h>
#include "esp_dsc.h"


void HandleChanRA();
void HandleChanDEC();
int ParseEncoderRA();
int ParseEncoderDEC();

void
setup() {
    Serial.begin(SERIAL_SPEED);
    pinMode(CHAN_RA_A, INPUT_PULLUP);
    pinMode(CHAN_RA_B, INPUT_PULLUP);
    pinMode(CHAN_DEC_A, INPUT_PULLUP);
    pinMode(CHAN_DEC_B, INPUT_PULLUP);
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
loop()
{ 
    Serial.print("RA Encoder Ticks: ");
    Serial.print(_RAEncoderTicks);
    Serial.print("  Revolutions: ");
    Serial.print(_RAEncoderTicks/RA_RESOLUTION);
    Serial.print("\n");
    Serial.print("DEC Encoder Ticks: ");
    Serial.print(_DECEncoderTicks);
    Serial.print("  Revolutions: ");
    Serial.print(_DECEncoderTicks/DEC_RESOLUTION);
    Serial.print("\n");
    delay(100);
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
