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

#ifndef _ARGENTUM_H_
#define _ARGENTUM_H_

#include "../util/stepper.h"
#include "../util/axis.h"
#include "../util/limit.h"
#include "../util/rollers.h"
#include "../util/SerialCommand.h"
#include "../util/SdFat/SdFat.h"

// All pin mappings should go here
#define FET_1_PIN             7
#define FET_2_PIN             8
#define FET_3_PIN             9

#define PIN_FET_1             7
#define PIN_FET_2             8
#define PIN_FET_3             9

#define PIN_FET_RED           3
#define PIN_FET_GREEN         4
#define PIN_FET_BLUE          2

// Unfortunately ROLLER_SERVO_PIN was not seen from rollers.h and had to be duplicated there.
#define ROLLER_SERVO_PIN      10	// AOUT 1
#define AOUT_2_PIN            11  // Spare, not yet used.
#define AOUT_3_PIN            12  // Spare, not yet used.

#define STEPPER_A_STEP_PIN    15
#define STEPPER_A_DIR_PIN     14
#define STEPPER_A_ENABLE_PIN  16

#define STEPPER_B_STEP_PIN    18
#define STEPPER_B_DIR_PIN     17
#define STEPPER_B_ENABLE_PIN  19

#define PIN_LIMIT_X_POSITIVE  5
#define PIN_LIMIT_X_NEGATIVE  A0
#define PIN_LIMIT_Y_POSITIVE  A1
#define PIN_LIMIT_Y_NEGATIVE  6

#define PIN_ANALOG_1          A12
#define PIN_ANALOG_2          A13
#define PIN_ANALOG_3          A14
#define PIN_PRIMITIVE_VOLTAGE A15

extern Stepper a_motor;
extern Stepper b_motor;

extern Axis x_axis;
extern Axis y_axis;

extern SerialCommand serial_command;
extern Rollers rollers;

extern long x_size;
extern long y_size;

extern SdFat sd;

#endif
