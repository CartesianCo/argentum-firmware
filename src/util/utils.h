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

#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

#define RAM_TOTAL 8192

int ram_free(void);
int ram_used(void);
double ram_utilisation(void);

uint8_t CRC8(const void *buffer, const uint16_t length);
uint8_t CRC8_add_byte(uint8_t data, uint8_t seed_crc);

void print_switch_status(void);
void print_switch_status(uint8_t switches);

void fet_initialise(void);
void fet_set_value(uint8_t fet, uint8_t value);

void analog_initialise(void);
uint16_t analog_read(uint8_t analog);

double primitive_voltage(void);
bool no_power(void);

#endif
