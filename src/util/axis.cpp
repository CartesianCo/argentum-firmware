#include "axis.h"
#include "logging.h"

#include "../argentum/argentum.h"

Axis::Axis(const char axis,
           Stepper *motor,
           bool (*positive_limit)(void),
           bool (*negative_limit)(void)) {
    this->axis = axis;
    this->motor = motor;
    this->positive_limit = positive_limit;
    this->negative_limit = negative_limit;

    length = 0;
    current_position = 0;
    desired_position = 0;

    motor_mapping = Axis::CW_Positive;

    direction = Axis::Positive;

    motor->set_speed(1000);

    //logger.info() << "Axis created for: " << axis << Comms::endl;
}

Axis::~Axis() {
}

bool Axis::run(void) {
    if(current_position == desired_position) {
        return false;
    } else {
        //logger.info() << current_position << " -> " << desired_position
        //        << Logger::endl;

        /*if(desired_position == Axis::PositiveLimit) {
            if(positive_limit()) {
                logger.info("reached positive limit");
            } else {
                return step();
            }
        }*/

        if(((current_position < desired_position) && positive_limit())
                || ((current_position > desired_position)
                && negative_limit())) {
            logger.warn() << axis
                    << " tried to step in a limited direction, holding."
                    << " current_position: " << current_position
                    << " desired_position: " << desired_position
                    << Comms::endl;

            hold();

            return false;
        }

        return step();
    }
}

bool Axis::step(void) {
    bool did_step = motor->step();

    if(did_step) {
        //logger.info() << axis << " step " << current_position << Comms::endl;

        if(direction == Axis::Positive) {
            current_position++;
        } else {
            current_position--;
        }

        /*if(current_position == desired_position) {
            logger.info() << axis << " axis reached goal position: "
                    << desired_position << Comms::endl;
        }*/
    }

    return did_step;
}

void Axis::set_direction(uint8_t direction) {
    if(direction == this->direction) {
        return;
    }

    this->direction = direction;
    //motor->swap_direction();

    if(direction == Axis::Positive) {
        if(motor_mapping == Axis::CW_Positive) {
            motor->set_direction(Stepper::CW);
        } else {
            motor->set_direction(Stepper::CCW);
        }
    } else if(direction == Axis::Negative) {
        if(motor_mapping == Axis::CW_Negative) {
            motor->set_direction(Stepper::CW);
        } else {
            motor->set_direction(Stepper::CCW);
        }
    }

    //logger.info() << axis << " axis setting direction to " << direction
    //        << Comms::endl;
}

void Axis::move_absolute(double position) {
    if(position < 0) {
        logger.error() << axis << " absolute movement with negative position ("
                << position << ")";
    }

    uint32_t pos = position * steps_per_mm;

    move_absolute(pos);
}

void Axis::move_absolute(uint32_t position) {
    if(position == desired_position || position == current_position) {
        return;
    }

    //logger.info() << axis << " axis absolute movement from " << current_position
    //    << " to " << position << "" << Comms::endl;

    // Constrain the possible positions
    desired_position = max(position, 0);

    // This could really be ~14000
    desired_position = min(desired_position, 16000);

    //logger.info() << axis << " axis setting new desired position to "
    //    << desired_position << Comms::endl;

    if(desired_position > current_position) {
        set_direction(Axis::Positive);
    } else {
        set_direction(Axis::Negative);
    }
}

void Axis::move_incremental(double increment) {
    int32_t steps = increment * steps_per_mm;

    move_incremental(steps);
}

void Axis::move_incremental(int32_t increment) {
    uint32_t new_desired_position = desired_position + increment;

    //logger.info() << axis << " axis given increment of (" << increment
    //        << ")" << Comms::endl;

    if(((int32_t)desired_position + increment) < 0) {
        logger.error() << axis << " axis given incremental move below 0.000 ("
                << increment << ")" << Comms::endl;

        new_desired_position = 0;
    }

    move_absolute(new_desired_position);
}

void Axis::move_to_positive(void) {
    set_direction(Axis::Positive);

    while(!positive_limit()) {
        while(!step());
    }
}

void Axis::move_to_negative(void) {
    set_direction(Axis::Negative);

    while(!negative_limit()) {
        while(!step());
    }
}

double Axis::get_current_position_mm(void) {
    return ((double)current_position) / steps_per_mm;
}

double Axis::get_desired_position_mm(void) {
    return ((double)desired_position) / steps_per_mm;
}

uint32_t Axis::get_current_position(void) {
    return current_position;
}

uint32_t Axis::get_desired_position(void) {
    return desired_position;
}

void Axis::zero(void) {
    current_position = 0;
    desired_position = 0;
}

void Axis::hold(void) {
    desired_position = current_position;
}

bool Axis::moving(void) {
    return (current_position != desired_position);
}

void Axis::wait_for_move(void) {
    while(moving()) {
        run();
    }
}

void Axis::set_speed(uint32_t mm_per_minute) {
    motor->set_speed(mm_per_minute);
}

uint8_t Axis::get_motor_mapping(void) {
    return motor_mapping;
}

void Axis::set_motor_mapping(uint8_t motor_mapping) {
    this->motor_mapping = motor_mapping;

    // TODO: This is duplicated here and in the direction function. There's a
    // better way of structuring it.
    if(direction == Axis::Positive) {
        if(motor_mapping == Axis::CW_Positive) {
            motor->set_direction(Stepper::CW);
        } else {
            motor->set_direction(Stepper::CCW);
        }
    } else if(direction == Axis::Negative) {
        if(motor_mapping == Axis::CW_Negative) {
            motor->set_direction(Stepper::CW);
        } else {
            motor->set_direction(Stepper::CCW);
        }
    }
}

void Axis::set_motor(Stepper *motor) {
    this->motor = motor;

    this->motor->set_direction(Stepper::CW);
}

Stepper * Axis::get_motor(void) {
    return motor;
}

void Axis::debug_info(void) {
    LoggerWrapper &info = logger.info() << axis << " axis, ";

    if(motor == &a_motor) {
        info << "a";
    } else {
        info << "b";
    }

    info << " motor, ";

    if(direction == Axis::Positive) {
        info << "+";
    } else {
        info << "-";
    }

    info << " direction, ";

    if(motor->get_direction() == Stepper::CW) {
        info << "CW";
    } else {
        info << "CCW";
    }

    info << " motor";

    if(motor_mapping == CW_Positive) {
        info << " (+CW STD)";
    } else {
        info << " (-CW INV)";
    }

    info << Comms::endl;
}
