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

#ifndef _AXIS_H_
#define _AXIS_H_

#include "Motor.h"

class Axis {
public:
    enum Axes {
        X = 'X',
        Y = 'Y',
    };

    enum {
        Positive = 0,
        Negative = 1
    };

    // Enumeration for the mapping of this axis' direction to the underlying
    // motor direction.
    enum MotorMapping {
        NonInverted = 0,
        Inverted = 1
    };

    Axis();
    ~Axis();

    bool run(void);

    void move_to(uint32_t position);
    void set_direction(uint8_t direction);

private:

    void step(void);

    Motor *motor;
    uint8_t direction;
    uint32_t length;

    uint8_t motor_mapping;

    uint32_t current_position;
    uint32_t desired_position;
};

#endif
