//
//  Motor.h
//
//
//  Created by Isabella Stephens on 10/12/12.
//  Copyright (c) 2012 Cartesian Co. All rights reserved.
//

#ifndef _Motor_h
#define _Motor_h

#include "Arduino.h"

class Motor {
public:
    enum Motors {
        A = 'A',
        B = 'B',
    };

    Motor(int step_pin, int dir_pin, int power_pin, int steps_per_rev);
    Motor();
    void power(bool state);
    void switch_power();

    long get_position(void);
    void set_position(long position);
    void reset_position(void);

    void set_direction(uint8_t direction);
    void swap_direction(void);

    void set_inverted(bool inverted);
    bool is_inverted(void);

    void step();
    void steps(long steps);
    void move(long steps);
    void go_home(void);

    void set_speed(int mm_per_minute); //set to 0 for instantaneous movement
    int  get_speed();

    long last_step_time;

    static const uint8_t Forward = 0;
    static const uint8_t Backward = 1;

private:
    int step_pin;
    int dir_pin;
    int power_pin;
    int steps_per_rev;
    double position;
    int direction;
    int speed;
    bool inverted;

    static const long steps_per_meter = 80000; //0.0125 mm per step
};

#endif
