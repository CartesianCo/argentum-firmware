#include "argentum.h"

ProtoMotor xProto(15, 14, 16);
ProtoMotor yProto(18, 17, 19);

Axis x_axis(Axis::X, &xProto, &limit_x_positive, &limit_x_negative);
Axis y_axis(Axis::Y, &yProto, &limit_y_positive, &limit_y_negative);

SerialCommand serial_command;

Rollers rollers;
