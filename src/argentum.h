#ifndef _ARGENTUM_H_
#define _ARGENTUM_H_

#include "stepper.h"
#include "axis.h"
#include "limit.h"
#include "rollers.h"
#include "SerialCommand.h"

extern Stepper a_motor;
extern Stepper b_motor;

extern Axis x_axis;
extern Axis y_axis;

extern SerialCommand serial_command;
extern Rollers rollers;

extern long x_size;
extern long y_size;

#endif
