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

#include "stepper.h"

#include <stdint.h>

Stepper::Stepper(int step_pin, int dir_pin, int enable_pin) {
    this->step_pin = step_pin;
    this->dir_pin = dir_pin;
    this->enable_pin = enable_pin;

    this->last_step_time = 0;

    /* Speed is the delay between steps necessary to move at the required speed
     * The default speed is 500. To move instantaneously, set speed to 0 (this
     * will cause the motors to skip). Speed is not used by functions which step
     * only once.
     */
    set_speed(500);

    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);

    digitalWrite(step_pin, LOW);

    set_direction(Stepper::CW);

    enable(true);
}

void Stepper::enable(bool enabled) {
    if (enabled) {
        digitalWrite(enable_pin, LOW);
    } else {
        digitalWrite(enable_pin, HIGH);
    }
}

uint8_t Stepper::swap_direction(void) {
    this->set_direction(!this->direction);

    return this->direction;
}

void Stepper::set_direction(uint8_t direction) {
    this->direction = direction;

    if (direction) {
        digitalWrite(dir_pin, HIGH);
    } else {
        digitalWrite(dir_pin, LOW);
    }
}

uint8_t Stepper::get_direction(void) {
    return direction;
}

bool Stepper::step() {
    if((micros() - last_step_time) > step_delay) {
        digitalWrite(step_pin, HIGH);
        digitalWrite(step_pin, LOW);

        last_step_time = micros();

        return true;
    }

    return false;
}

void Stepper::set_speed(int mm_per_minute) {
    int rate = mm_per_minute;

    if (rate > 5000) {
        rate = 5000; //maximum rate
    } else if (rate < 0) {
        rate = 0; // cannot have a negative rate
    }

    long i = 1000000/((rate * steps_per_mm)/60);

    step_delay = i;
}

int Stepper::get_speed() {
    return 0; //speed;
}
