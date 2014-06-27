#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Motor.h"

const static int a_escape_steps = 400;
const static int b_escape_steps = 400;

extern Motor *xMotor;
extern Motor *yMotor;

extern Motor aMotor;
extern Motor bMotor;

void calibration(void);

#endif
