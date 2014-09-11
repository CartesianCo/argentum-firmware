#include "argentum.h"

// Should be x axis
Stepper a_motor(STEPPER_A_STEP_PIN, STEPPER_A_DIR_PIN, STEPPER_A_ENABLE_PIN);

// Should be y axis
Stepper b_motor(STEPPER_B_STEP_PIN, STEPPER_B_DIR_PIN, STEPPER_B_ENABLE_PIN);

Axis x_axis(Axis::X, &a_motor, &limit_x_positive, &limit_x_negative);
Axis y_axis(Axis::Y, &b_motor, &limit_y_positive, &limit_y_negative);

SerialCommand serial_command;

Rollers rollers;

Servo servo1;
Servo servo2;
Servo servo3;

Servo *servos[] = {
    &servo1,
    &servo2,
    &servo3
};

uint8_t servo_pins[] = {
    PIN_SERVO_1,
    PIN_SERVO_2,
    PIN_SERVO_3
};

SdFat sd;

long x_size = 0;
long y_size = 0;

Axis * axis_from_id(uint8_t id) {
    id = toupper(id);

    switch(id) {
        case Axis::X:
            return &y_axis;
            break;

        case Axis::Y:
            return &x_axis;
            break;

        default:
            return NULL;
    }
}

Stepper * motor_from_axis(unsigned const char axis) {
    if (toupper(axis) == 'X') {
        return x_axis.get_motor();
    } else if (toupper(axis) == 'Y') {
        return y_axis.get_motor();
    }

    return NULL;
}


void load_settings(void) {
    // Initialise Axes from EEPROM here
    if(global_settings.stepper_calibration.x_axis.motor == 'A') {
        x_axis.set_motor(&a_motor);
    } else {
        x_axis.set_motor(&b_motor);
    }

    if(global_settings.stepper_calibration.y_axis.motor == 'A') {
        y_axis.set_motor(&a_motor);
    } else {
        y_axis.set_motor(&b_motor);
    }

    if(global_settings.stepper_calibration.x_axis.flipped) {
        x_axis.set_motor_mapping(Axis::CW_Negative);
    } else {
        x_axis.set_motor_mapping(Axis::CW_Positive);
    }

    if(global_settings.stepper_calibration.y_axis.flipped) {
        y_axis.set_motor_mapping(Axis::CW_Negative);
    } else {
        y_axis.set_motor_mapping(Axis::CW_Positive);
    }

    x_axis.length = global_settings.stepper_calibration.x_axis.length;
    y_axis.length = global_settings.stepper_calibration.y_axis.length;
}
