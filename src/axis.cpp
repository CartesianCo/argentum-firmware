#include "axis.h"

Axis::Axis() {
    length = 0;
    current_position = 0;
    desired_position = 0;

    motor_mapping = Axis::NonInverted;

    direction = Axis::Positive;
}

Axis::~Axis() {
}

bool Axis::run(void) {
    if(current_position == desired_position) {
        return false;
    }

    step();

    return true;
}

void Axis::step(void) {
    motor->step();

    if(direction = Axis::Positive) {
        current_position++;
    } else {
        current_position--;
    }
}

void Axis::set_direction(uint8_t direction) {
    if(direction == this->direction) {
        return;
    }

    this->direction = direction;

    /*if(direction == Axis::Positive) {
        if(motor_mapping == Axis::NonInverted) {
            motor->set_direction(Motor::CW);
        } else {
            motor->set_direction(Motor::CCW);
        }
    } else {
        if(motor_mapping == Axis::NonInverted) {
            motor->set_direction(Motor::CCW);
        } else {
            motor->set_direction(Motor::CW);
        }
    }*/
}

void Axis::move_to(uint32_t position) {
    if(position == desired_position || position == current_position) {
        return;
    }

    desired_position = position;

    if(desired_position > current_position) {
        set_direction(Axis::Positive);
    } else {
        set_direction(Axis::Negative);
    }
}
