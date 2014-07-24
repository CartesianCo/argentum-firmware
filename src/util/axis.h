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

#include "stepper.h"

class Axis {
public:
    enum Axes {
        X = 'X',
        Y = 'Y',
    };

    enum StepperMapping {
        CW_Positive =  0,
        CW_Negative =  1,
        CCW_Positive = CW_Negative,
        CCW_Negative = CW_Positive
    };

    enum Goals {
        PositiveLimit =  123456789,
        NegativeLimit = -123456789
    };

    Axis(const char axis, Stepper *motor, bool (*positive_limit_function)(void), bool (*negative_limit_function)(void));
    ~Axis();

    bool run(void);

    void move_absolute(double position);
    void move_absolute(uint32_t position);

    void move_incremental(double increment);
    void move_incremental(int32_t increment);

    void move_to_positive(void);
    void move_to_negative(void);

    double get_current_position(void);
    double get_desired_position(void);

    void zero(void);
    void hold(void);

    bool moving(void);
    void wait_for_move(void);

    void set_speed(uint32_t mm_per_minute);

    void set_motor_mapping(uint8_t motor_mapping);
    uint8_t get_motor_mapping(void);

    void set_motor(Stepper *motor);
    Stepper * get_motor(void);

    static const long steps_per_mm = 80;

    uint32_t current_position;

    void debug_info(void);

private:
    enum StepDirection {
        Positive = 0,
        Negative = 1
    };

    bool step(void);
    void set_direction(uint8_t direction);

    char axis;

    bool (*positive_limit)(void);
    bool (*negative_limit)(void);

    Stepper *motor;

    uint8_t direction;
    uint32_t length;

    uint8_t motor_mapping;

    uint32_t desired_position;
};

#endif
