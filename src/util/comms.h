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

#ifndef _COMMS_H_
#define _COMMS_H_

#include <stdint.h>
#include <Arduino.h>

namespace Comms {
    static const char *endl = "\r\n";
}

class SerialChannel {
public:
    SerialChannel(uint32_t baudrate);
    ~SerialChannel();

    void initialise(void);

    void write(const void *data, uint8_t length);

    template<class T> void send(const T arg) {
        Serial.print(arg);
    }

    template<class T> void println(const T arg) {
        send(arg);
        send(Comms::endl);
    }

    void println(void);

private:
    uint32_t baudrate;
};

extern SerialChannel comms;

#endif
