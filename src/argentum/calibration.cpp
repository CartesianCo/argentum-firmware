#include "calibration.h"

#include "../util/limit.h"
#include "../util/settings.h"
#include "../util/axis.h"
#include "../util/stepper.h"
#include "../util/logging.h"
#include "../util/utils.h"

#include "commands.h"

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
    if(!motor) {
        return false;
    }

    uint8_t before = limit_switches();

    if(steps < 0) {
        motor->set_direction(Stepper::CCW);
        steps = abs(steps); // Don't like this particularly
    } else {
        motor->set_direction(Stepper::CW);
    }

    for(long step = 0; step < steps; step++) {
        while(!motor->step());
    }

    uint8_t after = limit_switches();

    uint8_t released = (before & ~after);
    uint8_t triggered = (after & ~before);

    // If we activated a switch, release it. Should we do this?
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
            Axis *axis;

            if(axis_correct) {
                logger.info("Found A = X");

                axis = &x_axis;

                x_axis.set_motor(&a_motor);
                y_axis.set_motor(&b_motor);

                *x_direction_resolved = true;
            } else {
                logger.info("Found A = Y");

                axis = &y_axis;

                x_axis.set_motor(&b_motor);
                y_axis.set_motor(&a_motor);

                *y_direction_resolved = true;
            }

            if(direction_correct) {
                logger.info("Found + = +");

                axis->set_motor_mapping(Axis::CW_Positive);
            } else {
                logger.info("Found + = -");

                //a_motor.set_direction(Stepper::CCW);
                axis->set_motor_mapping(Axis::CW_Negative);
            }
        }

        // B -> Y+
        b_resolved = resolve(&b_motor, b_escape_steps, Y_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!b_resolved) {
            b_resolved = resolve(&b_motor, -b_escape_steps, Y_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(b_resolved) {
            Axis *axis;

            if(axis_correct) {
                logger.info("Found B = Y");

                axis = &y_axis;

                x_axis.set_motor(&a_motor);
                y_axis.set_motor(&b_motor);

                *y_direction_resolved = true;
            } else {
                logger.info("Found B = X");

                axis = &x_axis;

                x_axis.set_motor(&b_motor);
                y_axis.set_motor(&a_motor);

                *x_direction_resolved = true;
            }

            if(direction_correct) {
                logger.info("Found + = +");

                axis->set_motor_mapping(Axis::CW_Positive);
            } else {
                logger.info("Found + = -");

                //b_motor.set_direction(Stepper::CCW);
                axis->set_motor_mapping(Axis::CW_Negative);
            }
        }
    } else {
        logger.info("No switches are initially triggered.");
    }

    return (a_resolved | b_resolved);
}

void calibrate(StepperCalibrationData *calibration) {
    logger.info("Calibrating.");

    //logger.enabled = false;

    x_axis.set_speed(250);
    y_axis.set_speed(250);

    x_axis.set_motor_mapping(Axis::CW_Positive);
    y_axis.set_motor_mapping(Axis::CW_Positive);

    x_axis.get_motor()->set_direction(Stepper::CW);
    y_axis.get_motor()->set_direction(Stepper::CW);

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
            while(!x_axis.get_motor()->step());
        }

        if(limit_y()) {
            logger.info("Found axes swapped.");
            // Incorrect mapping
            Stepper *temp = x_axis.get_motor();

            x_axis.set_motor(y_axis.get_motor());
            y_axis.set_motor(temp);
        }
    } else {
        if(!x_direction_resolved) {
            logger.info("Failed to resolve X");

            x_axis.get_motor()->set_direction(Stepper::CW);

        }

        if(!y_direction_resolved) {
            logger.info("Failed to resolve Y");

            y_axis.get_motor()->set_direction(Stepper::CW);
        }
    }

    while(!(x_direction_resolved && y_direction_resolved)) {
        if(!x_direction_resolved) {
            //logger.info("Stepping X");

            if(!limit_x()) {
                x_axis.get_motor()->step();
            } else {
                x_direction_resolved = true;

                limit_switch_command();

                if(limit_x_negative()) {
                    // Inverted
                    logger.info("Found X to be inverted.");
                    x_axis.set_motor_mapping(Axis::CW_Negative);
                } else {
                    x_axis.set_motor_mapping(Axis::CW_Positive);
                }
            }
        }

        if(!y_direction_resolved) {
            if(!limit_y()) {
                //logger.info("Stepping Y");
                y_axis.get_motor()->step();
            } else {
                y_direction_resolved = true;

                limit_switch_command();

                if(limit_y_negative()) {
                    // Inverted
                    logger.info("Found Y to be inverted.");
                    y_axis.set_motor_mapping(Axis::CW_Negative);
                } else {
                    y_axis.set_motor_mapping(Axis::CW_Positive);
                }
            }
        }
    }

    // At this point we know both axes, and have resolved their directions.
    logger.info("Homing");

    x_axis.move_to_negative();
    logger.info("x-");
    y_axis.move_to_negative();
    logger.info("y-");

    x_axis.zero();
    y_axis.zero();

    // Go to maximum extents
    x_axis.move_to_positive();
    logger.info("x+");
    y_axis.move_to_positive();
    logger.info("y+");

    x_distance = x_axis.get_current_position();
    y_distance = y_axis.get_current_position();

    // Return to home
    x_axis.move_to_negative();
    logger.info("x-");
    y_axis.move_to_negative();
    logger.info("y-");

    x_axis.zero();
    y_axis.zero();

    // Why doesn't this work?
    //x_axis.current_position = 100000;
    //x_axis.move_absolute(0.000);

    if(calibration) {
        calibration->x_axis.motor = (x_axis.get_motor() == &a_motor) ? 'A' : 'B';
        calibration->x_axis.flipped = (x_axis.get_motor_mapping() != Axis::CW_Positive);
        calibration->x_axis.length = x_distance;

        calibration->y_axis.motor = (y_axis.get_motor() == &a_motor) ? 'A' : 'B';
        calibration->y_axis.flipped = (y_axis.get_motor_mapping() != Axis::CW_Positive);
        calibration->y_axis.length = y_distance;
    }

    logger.enabled = true;

    logger.info("Done.");
}
