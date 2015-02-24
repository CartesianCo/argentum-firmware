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

#include "argentum.h"

// Should be x axis
Stepper a_motor(STEPPER_A_STEP_PIN, STEPPER_A_DIR_PIN, STEPPER_A_ENABLE_PIN);

// Should be y axis
Stepper b_motor(STEPPER_B_STEP_PIN, STEPPER_B_DIR_PIN, STEPPER_B_ENABLE_PIN);

Axis x_axis(Axis::X, &a_motor, &limit_x_positive, &limit_x_negative);
Axis y_axis(Axis::Y, &b_motor, &limit_y_positive, &limit_y_negative);

SerialCommand serial_command;

Rollers rollers;

SdFat sd;

long x_size = 0;
long y_size = 0;
