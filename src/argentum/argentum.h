#ifndef _ARGENTUM_H_
#define _ARGENTUM_H_

#include "../util/stepper.h"
#include "../util/axis.h"
#include "../util/limit.h"
#include "../util/rollers.h"
#include "../util/SerialCommand.h"

// All pin mappings should go here
#define FET_1_PIN 7
#define FET_2_PIN 8
#define FET_3_PIN 9

#define STEPPER_A_STEP_PIN    15
#define STEPPER_A_DIR_PIN     14
#define STEPPER_A_ENABLE_PIN  16

#define STEPPER_B_STEP_PIN    18
#define STEPPER_B_DIR_PIN     17
#define STEPPER_B_ENABLE_PIN  19

extern Stepper a_motor;
extern Stepper b_motor;

extern Axis x_axis;
extern Axis y_axis;

extern SerialCommand serial_command;
extern Rollers rollers;

extern long x_size;
extern long y_size;

#endif
