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

#ifndef _LIMIT_H_
#define _LIMIT_H_

#include <stdint.h>

#define X_POS_HARDWARE_BIT 0b00001000
#define X_NEG_HARDWARE_BIT 0b00000001
#define Y_POS_HARDWARE_BIT 0b00000010
#define Y_NEG_HARDWARE_BIT 0b00001000

#define X_POS_BIT 0b00001000
#define X_NEG_BIT 0b00000100
#define Y_POS_BIT 0b00000010
#define Y_NEG_BIT 0b00000001

#define X_MASK    0b00001100
#define Y_MASK    0b00000011
#define POS_MASK  0b00001010
#define NEG_MASK  0b00000101

#define X_POS(switches) (switches & X_POS_BIT)
#define X_NEG(switches) (switches & X_NEG_BIT)
#define Y_POS(switches) (switches & Y_POS_BIT)
#define Y_NEG(switches) (switches & Y_NEG_BIT)

#define X_LIMIT(switches) (switches & X_MASK)
#define Y_LIMIT(switches) (switches & Y_MASK)

#define POS_LIMIT(switches) (switches & POS_MASK)
#define NEG_LIMIT(switches) (switches & NEG_MASK)

/*
class LimitSwitch {
public:
    LimitSwitch(volatile uint8_t *port, uint8_t bit);
    ~LimitSwitch();

    bool triggered(void);

private:
    volatile uint8_t *port;
    uint8_t bit;
};
*/

extern bool limit_switch_nc;

void limit_initialise(void);

uint8_t limit_switches(void);

bool limit_x_positive(void);
bool limit_x_negative(void);
bool limit_y_positive(void);
bool limit_y_negative(void);

bool limit_x(void);
bool limit_y(void);

bool limit_positive(void);
bool limit_negative(void);

bool limit_any(void);

#endif
