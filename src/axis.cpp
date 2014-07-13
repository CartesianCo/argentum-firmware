#include "axis.h"
#include "logging.h"

Axis::Axis(const char axis, ProtoMotor *motor, bool (*positive_limit)(void), bool (*negative_limit)(void)) {
    this->axis = axis;
    this->motor = motor;
    this->positive_limit = positive_limit;
    this->negative_limit = negative_limit;

    length = 0;
    current_position = 0;
    desired_position = 0;

    motor_mapping = Axis::NonInverted;

    direction = Axis::Positive;

    motor->set_speed(5000);

    logger.info() << "Axis created for: " << axis << Logger::endl;
}

Axis::~Axis() {
}

bool Axis::run(void) {
    if(current_position == desired_position) {
        return false;
    } else {
        //logger.info() << current_position << " -> " << desired_position << Logger::endl;
        if(positive_limit() || negative_limit()) {
            logger.warn("Tried to step with a limit triggered.");
            return false;
        }

        return step();
    }
}

bool Axis::step(void) {
    bool did_step = motor->step();

    if(did_step) {
        //logger.info("Step");
        if(direction == Axis::Positive) {
            current_position++;
        } else {
            current_position--;
        }

        if(current_position == desired_position) {
            logger.info() << "Axis " << axis << " reached goal position: " << desired_position << Logger::endl;
        }
        return true;
    }

    return false;
}

void Axis::set_direction(uint8_t direction) {
    this->direction = direction;

    logger.info() << "Setting direction to " << direction << Logger::endl;
}

void Axis::move_absolute(double position) {
    uint32_t pos = position * steps_per_mm;

    logger.info() << "move_to(" << position << ") -> move_to(" << pos << ")" << Logger::endl;

    move_absolute(pos);
}

void Axis::move_absolute(uint32_t position) {
    if(position == desired_position || position == current_position) {
        return;
    }

    // Ensure we don't set a negative goal
    desired_position = max(position, 0);

    logger.info() << "Setting new desired position to " << position << Logger::endl;

    if(desired_position > current_position) {
        set_direction(Axis::Positive);
    } else {
        set_direction(Axis::Negative);
    }
}
