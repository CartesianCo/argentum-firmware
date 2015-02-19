#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "../util/settings.h"

const static int a_escape_steps = 400;
const static int b_escape_steps = 400;

bool calibrate(CalibrationData *calibration);

#endif
