#ifndef _ARGENTUM_H_
#define _ARGENTUM_H_

#include "../util/stepper.h"
#include "../util/axis.h"
#include "../util/limit.h"
#include "../util/rollers.h"
#include "../util/SerialCommand.h"
#include "../util/SdFat/SdFat.h"

// All pin mappings should go here
#define PIN_FET_1             7
#define PIN_FET_2             8
#define PIN_FET_3             9

#define PIN_FET_RED           3
#define PIN_FET_GREEN         2
#define PIN_FET_BLUE          4

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

// Helper functions

Axis * axis_from_id(uint8_t id);
Stepper * motor_from_axis(unsigned const char axis);

#endif
