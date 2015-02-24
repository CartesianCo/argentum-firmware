/*
    Argentum Firmware

    Copyright (C) 2013 Isabella Stevens
    Copyright (C) 2014 Michael Shiel
    Copyright (C) 2015 Trent Waddington

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "colour.h"

#include "../argentum/argentum.h"

// LED Control Functions
void colour_init(void) {
    pinMode(PIN_FET_RED, OUTPUT);
    pinMode(PIN_FET_GREEN, OUTPUT);
    pinMode(PIN_FET_BLUE, OUTPUT);

    colour(0);
}

void colour(uint32_t colour) {
    uint8_t red = RED_COMPONENT(colour);
    uint8_t green = GREEN_COMPONENT(colour);
    uint8_t blue = BLUE_COMPONENT(colour);

    colour_rgb(red, green, blue);
}

void colour_rgb(uint8_t red, uint8_t green, uint8_t blue) {
    colour_red(red);
    colour_green(green);
    colour_blue(blue);
}

void colour_red(uint8_t red) {
    if(red > 0) {
        //digitalWrite(RED_CHANNEL_PIN, HIGH);
        analogWrite(PIN_FET_RED, red);
    } else {
        digitalWrite(PIN_FET_RED, LOW);
    }
}

void colour_green(uint8_t green) {
    if(green > 0) {
        digitalWrite(PIN_FET_GREEN, HIGH);
    } else {
        digitalWrite(PIN_FET_GREEN, LOW);
    }
}

void colour_blue(uint8_t blue) {
    if(blue > 0) {
        digitalWrite(PIN_FET_BLUE, HIGH);
    } else {
        digitalWrite(PIN_FET_BLUE, LOW);
    }
}
