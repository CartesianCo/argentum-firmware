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

#include "limit.h"
#include <Arduino.h>

#include "../argentum/argentum.h"

/*
LimitSwitch x_positive(&PINE, X_POS_HARDWARE_BIT);
LimitSwitch x_negative(&PINF, X_NEG_HARDWARE_BIT);
LimitSwitch y_positive(&PINF, Y_POS_HARDWARE_BIT);
LimitSwitch y_negative(&PINH, Y_NEG_HARDWARE_BIT);

LimitSwitch::LimitSwitch(volatile uint8_t *port, uint8_t bit) {
    this->port = port;
    this->bit = bit;
}

LimitSwitch::~LimitSwitch() {
}

bool LimitSwitch::triggered(void) {
    return *port & bit;
}
*/

void limit_initialise(void) {
    pinMode(PIN_LIMIT_X_POSITIVE, INPUT);
    pinMode(PIN_LIMIT_X_NEGATIVE, INPUT);

    pinMode(PIN_LIMIT_Y_POSITIVE, INPUT);
    pinMode(PIN_LIMIT_Y_NEGATIVE, INPUT);
}

bool limit_switch_nc = true;

uint8_t limit_switches(void) {
    uint8_t switches = 0b00000000;

    if(limit_x_positive()) {
        switches |= X_POS_BIT;
    }

    if(limit_x_negative()) {
        switches |= X_NEG_BIT;
    }

    if(limit_y_positive()) {
        switches |= Y_POS_BIT;
    }

    if(limit_y_negative()) {
        switches |= Y_NEG_BIT;
    }

    return switches;
}

bool limit_x_positive(void) {
    if(limit_switch_nc) {
        return !(PINE & 0b00001000);
    } else {
        return (PINE & 0b00001000);
    }
}

bool limit_x_negative(void) {
    if(limit_switch_nc) {
        return !(PINF & 0b00000001);
    } else {
        return (PINF & 0b00000001);
    }
}

bool limit_y_positive(void) {
    if(limit_switch_nc) {
        return !(PINF & 0b00000010);
    } else {
        return (PINF & 0b00000010);
    }
}

bool limit_y_negative(void) {
    if(limit_switch_nc) {
        return !(PINH & 0b00001000);
    } else {
        return (PINH & 0b00001000);
    }
}

bool limit_x(void) {
    return (limit_x_positive() || limit_x_negative());
}

bool limit_y(void) {
    return (limit_y_positive() || limit_y_negative());
}

bool limit_positive(void) {
    return (limit_x_positive() || limit_y_positive());
}

bool limit_negative(void) {
    return (limit_x_negative() || limit_y_negative());
}

bool limit_any(void) {
    return (limit_x() || limit_y());
}
