#include "argentum.h"

Stepper a_motor(15, 14, 16); // Should be x axis
Stepper b_motor(18, 17, 19); // Should be y axis

Axis x_axis(Axis::X, &a_motor, &limit_x_positive, &limit_x_negative);
Axis y_axis(Axis::Y, &b_motor, &limit_y_positive, &limit_y_negative);

SerialCommand serial_command;

Rollers rollers;

long x_size = 0;
long y_size = 0;
