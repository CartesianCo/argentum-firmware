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

#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "Arduino.h"

class Stepper {
public:
    enum {
        CW = 0,
        CCW = 1
    };

    Stepper(int step_pin, int dir_pin, int enable_pin);

    void enable(bool enabled);

    void set_direction(uint8_t direction);
    uint8_t get_direction(void);
    uint8_t swap_direction(void);

    bool step();

    void set_speed(int mm_per_minute); //set to 0 for instantaneous movement
    int  get_speed();

    static const long steps_per_mm = 80;

private:
    int step_pin;
    int dir_pin;
    int enable_pin;

    long last_step_time;
    int direction;
    int step_delay;
};

#endif
