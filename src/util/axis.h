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

#ifndef _AXIS_H_
#define _AXIS_H_

#include "utils.h"
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

    double get_current_position_mm(void);
    double get_desired_position_mm(void);
    uint32_t get_current_position(void);
    uint32_t get_desired_position(void);

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

    uint32_t start_position;
    uint32_t current_position;

    uint32_t length;

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

    uint8_t motor_mapping;

    uint32_t desired_position;
    uint32_t desired_speed;
};

#endif
