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

#include "stepper.h"

#include "logging.h"

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

    if (rate > 30000)
        rate = 30000; //maximum rate
    if (rate < 0)
        rate = 0; // cannot have a negative rate

    long i = 1000000/((rate * steps_per_mm)/60);

    step_delay = i;
}

int Stepper::get_speed() {
    return 0; //speed;
}
