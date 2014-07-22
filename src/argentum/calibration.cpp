#include "calibration.h"
//#include "limit_switch.h"
#include "../util/limit.h"

#include "../util/settings.h"
#include "../util/axis.h"
#include "../util/stepper.h"

#include "../util/logging.h"

#include "argentum.h"

/**
 * Attempt to resolve the direction and a axis of a motor.
 *
 * Assumptions:
 *
 * Only one switch is affected by the movement, and the axes are long enough such
 * that the supplied movement distance won't release and trigger the limits of
 * that axis.
 *
 * The (released | triggered) logic simplification relies on this.
 *
 * @param motor The motor to attempt to resolve
 * @param steps The number of steps to move that motor
 * @param axis_mask A mask defining the axis that this motor should correspond
 to.

 * @param axis_correct A pointer to a bool, which will indicate whether the
 axis_mask corresponds to any of the limit switches that were triggered or
 released.

 * @param direction_mask A mask defining the direction that this motor should
 move in for the given step, typically positive.

 * @param direction_correct A pointer to a bool...
 */
bool resolve(Stepper *motor,
             long steps,
             uint8_t axis_mask,
             bool *axis_correct,
             uint8_t direction_mask,
             bool *direction_correct) {

    uint8_t before = limit_switches();

    if(steps < 0) {
        motor->set_direction(Stepper::CCW);
    } else {
        motor->set_direction(Stepper::CW);
    }

    for(long step = 0; step < steps; step++) {
        while(!motor->step());
    }

    uint8_t after = limit_switches();

    uint8_t released = (before & ~after);
    uint8_t triggered = (after & ~before);

    // If we activated a switch, release it. Should this happen?
    /*if(triggered) {
        motor->move(-steps);
    }*/

    if(released || triggered) {
        *axis_correct = ((released | triggered) & axis_mask);

        *direction_correct = ((released | triggered) & direction_mask);

        if(released) {
            *direction_correct = !(*direction_correct);
        }

        return true;
    }

    return false;
}

bool freedom(bool *x_direction_resolved, bool *y_direction_resolved) {
    *x_direction_resolved = false;
    *y_direction_resolved = false;

    bool a_resolved = false;
    bool b_resolved = false;

    if(limit_any()) {
        bool axis_correct = false;
        bool direction_correct = false;

        // A -> X+
        a_resolved = resolve(&a_motor, a_escape_steps, X_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!a_resolved) {
            a_resolved = resolve(&a_motor, -a_escape_steps, X_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(a_resolved) {
            if(axis_correct) {
                logger.info("Found A = X");

                /*xMotor = &aMotor;
                yMotor = &bMotor;*/

                x_axis.motor = &a_motor;
                y_axis.motor = &b_motor;

                *x_direction_resolved = true;
            } else {
                logger.info("Found A = Y");

                /*xMotor = &bMotor;
                yMotor = &aMotor;*/

                x_axis.motor = &b_motor;
                y_axis.motor = &a_motor;

                *y_direction_resolved = true;
            }

            if(direction_correct) {
                logger.info("Found + = +");
            } else {
                logger.info("Found + = -");

                a_motor.set_direction(Stepper::CCW);
            }
        }

        // B -> Y+
        b_resolved = resolve(&b_motor, b_escape_steps, Y_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!b_resolved) {
            b_resolved = resolve(&b_motor, -b_escape_steps, Y_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(b_resolved) {
            if(axis_correct) {
                logger.info("Found B = Y");
                /*xMotor = &aMotor;
                yMotor = &bMotor;*/

                x_axis.motor = &a_motor;
                y_axis.motor = &b_motor;

                *y_direction_resolved = true;
            } else {
                logger.info("Found B = X");
                /*xMotor = &bMotor;
                yMotor = &aMotor;*/

                x_axis.motor = &b_motor;
                y_axis.motor = &a_motor;

                *x_direction_resolved = true;
            }

            if(direction_correct) {
                logger.info("Found + = +");
            } else {
                logger.info("Found + = -");
                b_motor.set_direction(Stepper::CCW);
            }
        }
    } else {
        logger.info("No switches are initially triggered.");
    }

    return (a_resolved | b_resolved);
}

void calibrate(CalibrationData *calibration) {
    logger.info("Calibration beginning.");

    //x_axis.set_direction(Axis::Positive);
    //y_axis.set_direction(Axis::Positive);
    x_axis.motor->set_direction(Stepper::CW);
    y_axis.motor->set_direction(Stepper::CW);

    x_axis.set_speed(250);
    y_axis.set_speed(250);

    long x_distance = 0L;
    long y_distance = 0L;

    bool axes_resolved = false;
    bool x_direction_resolved = false;
    bool y_direction_resolved = false;

    axes_resolved = freedom(&x_direction_resolved, &y_direction_resolved);

    x_axis.set_speed(1500);
    y_axis.set_speed(1500);

    if(!axes_resolved) {
        logger.info("Resolved nothing, finding X");

        while(!limit_any()) {
            while(!x_axis.motor->step());
        }

        if(limit_y()) {
            // Incorrect
            /*Motor *temp = xMotor;

            xMotor = yMotor;
            yMotor = temp;*/

            Stepper *temp = x_axis.motor;

            x_axis.motor = y_axis.motor;
            y_axis.motor = temp;
        }
    }

    while(!(x_direction_resolved && y_direction_resolved)) {
        if(!x_direction_resolved) {
            if(!limit_x()) {
                while(!x_axis.motor->step());
            } else {
                x_direction_resolved = true;

                if(limit_x_positive()) {
                    // Inverted
                    //xMotor->set_inverted(true);
                    x_axis.set_motor_mapping(Axis::CW_Negative);
                }
            }
        }

        if(!y_direction_resolved) {
            if(!limit_y()) {
                while(!y_axis.motor->step());
            } else {
                y_direction_resolved = true;

                if(limit_y_positive()) {
                    // Inverted
                    //yMotor->set_inverted(true);
                    y_axis.set_motor_mapping(Axis::CW_Negative);
                }
            }
        }
    }

    LoggerWrapper &info = logger.info() << "Homing: ";

    info << "1";

    //while(!limit_positive()) {
    while(!(limit_x_positive() && limit_y_positive())) {
        //x_axis.motor->step();
        //y_axis.motor->step();

        x_axis.move_incremental((int32_t)1);
        y_axis.move_incremental((int32_t)1);

        //x_distance++;
        //y_distance++;
    }

    info << "2";

    /*while(!limit_x_positive()) {
        x_axis.motor->step();

        x_distance++;
    }*/

    info << "3";

    /*while(!limit_y_positive()) {
        y_axis.motor->step();

        y_distance++;
    }*/

    info << "4";

    x_distance = 0;
    y_distance = 0;

    //x_axis.current_position = 100000;
    //y_axis.current_position = 100000;
/*
    while(!(limit_x_negative() && limit_y_negative())) {
        x_axis.move_incremental((int32_t)-1);
        y_axis.move_incremental((int32_t)-1);

        x_distance++;
        y_distance++;
    }
*/
    info << "5";

    /*while(!limit_x_negative()) {
        xMotor->move(-1);
        x_distance++;
    }*/

    info << "6";

    /*while(!limit_y_negative()) {
        yMotor->move(-1);
        y_distance++;
    }*/

    //xMotor->reset_position();
    //yMotor->reset_position();

    x_axis.move_absolute(0.000);
    y_axis.move_absolute(0.000);

    info << Comms::endl;

    if(calibration) {
        calibration->x_axis.motor = 0; //(xMotor == &aMotor) ? Motor::A : Motor::B;
        calibration->x_axis.flipped = false;//xMotor->is_inverted();
        calibration->x_axis.length = x_distance;

        calibration->y_axis.motor = 1; //(yMotor == &aMotor) ? Motor::A : Motor::B;
        calibration->y_axis.flipped = false;//yMotor->is_inverted();
        calibration->y_axis.length = y_distance;
    }
}
