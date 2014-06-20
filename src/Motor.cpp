//
//  Motor.cpp
//
//
//  Created by Isabella Stephens on 10/12/12.
//  Copyright (c) 2012 Cartesian Co. All rights reserved.
//

#include "Motor.h"

Motor::Motor(int step_pin, int dirPin, int powerPin, int stepsPerRev) {
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->power_pin = power_pin;
    this->steps_per_rev = steps_per_rev;
    this->direction = 0;
    this->position = 0;
    this->last_step_time = 0;

    /* Speed is the delay between steps necessary to move at the required speed
     * The default speed is 500. To move instantaneously, set speed to 0 (this
     * will cause the motors to skip). Speed is not used by functions which step
     * only once.
     */
    set_speed(1000);

    pinMode(step_pin,         OUTPUT);
    pinMode(dir_pin,          OUTPUT);
    pinMode(power_pin,        OUTPUT);

    digitalWrite(step_pin,    LOW);

    set_dir(Motor::Forward);

    power(true);
}

void Motor::power(bool state) {
    if (state) {
        digitalWrite(power_pin, LOW);
    } else {
        digitalWrite(power_pin, HIGH);
    }
}

long Motor::get_position() {
    return position;
}

void Motor::reset_position() {
    int current_direction = this->direction;

    move(position);

    set_dir(current_direction);
}

void Motor::set_dir(int direction) {
    this->direction = direction;
    if (direction) {
        digitalWrite(this->dir_pin, HIGH);
    } else {
        digitalWrite(this->dir_pin, LOW);
    }
}

void Motor::step() {
    while ((micros() - last_step_time) < speed) {
        if(micros() < last_step_time) {
            delay(speed);
            continue;
        }
    }

    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);

    last_step_time = micros();

    if (direction) {
        position--;
    } else {
        position++;
    }
}

void Motor::move(long steps) {
    long distance = steps;

    if (steps < 0) {
        set_dir(0);
        distance = steps * -1;
    }
    else {
        set_dir(1);
    }

    for (long i = 0; i < distance; i++) {
        step();
    }
}

void Motor::set_speed(int mm_per_minute) {
    int rate = mm_per_minute;
    if (rate > 5000) {
        rate = 5000; //maximum rate
    } else if (rate < 0) {
        rate = 0; // cannot have a negative rate
    }

    long i = 1000000000/((rate * steps_per_meter)/60);
    this->speed = i;
}

int Motor::get_speed() {
    return this->speed;
}
