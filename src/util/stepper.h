/*

The MIT License (MIT)

Copyright (c) 2014 Cartesian Co.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author: Michael Shiel

*/

#ifndef _STEPPER_H_
#define _STEPPER_H_

#include <Arduino.h>

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
