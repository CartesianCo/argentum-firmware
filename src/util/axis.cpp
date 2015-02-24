/*
    Argentum Firmware

    Copyright (C) 2013 Isabella Stevens
    Copyright (C) 2014 Michael Shiel
    Copyright (C) 2015 Trent Waddington

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    start_position = 0;
    current_position = 0;
    desired_position = 0;

    motor_mapping = Axis::CW_Positive;

    direction = Axis::Positive;

    set_speed(1000);

    //logger.info() << "Axis created for: " << axis << Comms::endl;
}

Axis::~Axis() {
}

bool Axis::run(void) {
    if (no_power())
    {
        desired_position = current_position;
        return false;
    }

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

        int acc_steps = 100;
        uint32_t min_speed = 100;
        uint32_t d_len = abs((int32_t)desired_position - (int32_t)start_position);
        if (d_len < acc_steps)
            acc_steps = d_len / 2;
        uint32_t d_from_start = abs((int32_t)current_position - (int32_t)start_position);
        uint32_t d_from_end = abs((int32_t)desired_position - (int32_t)current_position);
        if (d_from_start < acc_steps)
        {
            int speed = min_speed + (d_from_start + 1) * (desired_speed - min_speed) / acc_steps;
            motor->set_speed(speed);
        }
        else if (d_from_end < acc_steps)
        {
            int speed = min_speed + (d_from_end + 1) * (desired_speed - min_speed) / acc_steps;
            motor->set_speed(speed);
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
            if (current_position > 0)
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

    start_position = current_position;
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
    motor->set_speed(desired_speed);

    bool startingAtNegLimit = negative_limit();
    int steps = 0;
    while(!positive_limit()) {
        while(!step())
            if (no_power())
                break;
        if (no_power())
            break;

        if (startingAtNegLimit)
        {
            if (!negative_limit())
                startingAtNegLimit = false;
            else if (steps > 400)
            {
                // probably wrong direction calibration
                break;
            }
        }
        if (!startingAtNegLimit && negative_limit())
        {
            // probably bad calibration, stop
            break;
        }

        steps++;
    }
}

void Axis::move_to_negative(void) {
    set_direction(Axis::Negative);
    motor->set_speed(desired_speed);

    bool startingAtPosLimit = positive_limit();
    int steps = 0;
    while(!negative_limit())
    {
        while(!step())
            if (no_power())
                break;
        if (no_power())
            break;

        if (startingAtPosLimit)
        {
            if (!positive_limit())
                startingAtPosLimit = false;
            else if (steps > 400)
            {
                // probably wrong direction calibration
                break;
            }
        }
        if (!startingAtPosLimit && positive_limit())
        {
            // probably bad calibration, stop
            break;
        }

        steps++;
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
    desired_speed = mm_per_minute;
    motor->set_speed(desired_speed);
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
