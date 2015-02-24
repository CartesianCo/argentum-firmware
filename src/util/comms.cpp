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

#include "comms.h"

SerialChannel comms(115200);

SerialChannel::SerialChannel(uint32_t baudrate) {
    this->baudrate = baudrate;
}

SerialChannel::~SerialChannel() {
    Serial.end();
}

void SerialChannel::initialise(void) {
    Serial.begin(baudrate);
}

void SerialChannel::write(const void *data, uint8_t length) {
    Serial.write((const uint8_t *)data, length);
}

void SerialChannel::println(void) {
    Serial.println();
}
