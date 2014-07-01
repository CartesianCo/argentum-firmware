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

*/

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "Arduino.h"

class Motor {
public:
    enum {
        CW = 0,
        Clockwise = 0,

        CCW = 1,
        CounterClockwise = 1
    };

    Motor(int step_pin, int dir_pin, int enable_pin);

    void enable(bool enabled);

    void set_direction(uint8_t direction);
    uint8_t swap_direction(void);

    void step();

    void set_speed(int mm_per_minute); //set to 0 for instantaneous movement
    int  get_speed();

    static const long steps_per_meter = 80000; //0.0125 mm per step

private:
    int step_pin;
    int dir_pin;
    int enable_pin;

    long last_step_time;
    int direction;
    int speed;
};

#endif
