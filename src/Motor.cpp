//
//  Motor.cpp
//
//
//  Created by Isabella Stephens on 10/12/12.
//  Copyright (c) 2012 Cartesian Co. All rights reserved.
//

#include "Motor.h"

#include "logging.h"

Motor::Motor(int step_pin, int dir_pin, int power_pin, int steps_per_rev) {
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->power_pin = power_pin;
    this->steps_per_rev = steps_per_rev;
    this->direction = 0;
    this->position = 0;
    this->last_step_time = 0;
    this->inverted = false;

    /* Speed is the delay between steps necessary to move at the required speed
     * The default speed is 500. To move instantaneously, set speed to 0 (this
     * will cause the motors to skip). Speed is not used by functions which step
     * only once.
     */
    set_speed(500);

    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(power_pin, OUTPUT);

    digitalWrite(step_pin, LOW);

    set_direction(Motor::Forward);

    power(true);
}

Motor::Motor() {
    set_speed(500);
}

void Motor::power(bool state) {
    if (state) {
        digitalWrite(power_pin, LOW);
    } else {
        digitalWrite(power_pin, HIGH);
    }
}

long Motor::get_position(void) {
    return position;
}

void Motor::set_position(long position) {
    this->position = position;
}

void Motor::reset_position(void) {
    set_position(0L);
}

void Motor::swap_direction(void) {
    this->set_direction(!this->direction);
}

void Motor::set_direction(uint8_t direction) {
    this->direction = direction;

    if(this->inverted) {
        if (direction) {
            digitalWrite(dir_pin, LOW);
        } else {
            digitalWrite(dir_pin, HIGH);
        }
    } else {
        if (direction) {
            digitalWrite(dir_pin, HIGH);
        } else {
            digitalWrite(dir_pin, LOW);
        }
    }
}

void Motor::set_inverted(bool inverted) {
    this->inverted = inverted;

    this->set_direction(this->direction);
}

bool Motor::is_inverted(void) {
    return this->inverted;
}

void Motor::steps(long steps) {
    while(steps--) {
        this->step();
    }
}

void Motor::step() {
    while ((micros() - last_step_time) < speed) {
        if (micros() < last_step_time) {
            delay(speed);
            continue;
        }
    }

    digitalWrite(step_pin, HIGH);
    digitalWrite(step_pin, LOW);

    last_step_time = micros();

    if (direction == Motor::Forward) {
        position++;
    } else {
        position--;
    }
}

void Motor::move(long steps) {
    if (steps < 0) {
        set_direction(Motor::Backward);
        steps = -steps;
    }
    else {
        set_direction(Motor::Forward);
    }

    for (long i = 0; i < steps; i++) {
        step();
    }
}

void Motor::go_home(void) {
    int current_direction = this->direction;

    logger.info() << "Motor homing from " << position << Comms::endl;

    move(-position);

    set_direction(current_direction);
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
