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
    //constructor:
    Motor(int step_pin, int dir_pin, int power_pin, int steps_per_rev);
    Motor();
    void power(bool state);
    long get_position();
    void reset_position();
    void switch_power();

    void set_direction(uint8_t direction);
    void swap_direction(void);
    void set_inverted(bool inverted);

    void step();
    void steps(long steps);

    void set_speed(int mm_per_minute); //set to 0 for instantaneous movement
    int  get_speed();
    void move(long steps);
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
