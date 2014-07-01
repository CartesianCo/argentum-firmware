#include "calibration.h"
#include "limit_switch.h"

#include "settings.h"
#include "axis.h"

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
bool resolve(Motor *motor,
             long steps,
             uint8_t axis_mask,
             bool *axis_correct,
             uint8_t direction_mask,
             bool *direction_correct) {

    uint8_t before = limit_switches();
    motor->move(steps);
    uint8_t after = limit_switches();

    uint8_t released = (before & ~after);
    uint8_t triggered = (after & ~before);

    // If we activated a switch, release it. Should this happen?
    if(triggered) {
        motor->move(-steps);
    }

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

    if(any_limit()) {
        bool axis_correct = false;
        bool direction_correct = false;

        // A -> X+
        a_resolved = resolve(&aMotor, a_escape_steps, X_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!a_resolved) {
            a_resolved = resolve(&aMotor, -a_escape_steps, X_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(a_resolved) {
            if(axis_correct) {
                Serial.println("Found A = X");

                xMotor = &aMotor;
                yMotor = &bMotor;

                *x_direction_resolved = true;
            } else {
                Serial.println("Found A = Y");

                xMotor = &bMotor;
                yMotor = &aMotor;

                *y_direction_resolved = true;
            }

            if(direction_correct) {
                Serial.println("Found + = +");
            } else {
                Serial.println("Found + = -");
                aMotor.set_inverted(true);
            }
        }

        // B -> Y+
        b_resolved = resolve(&bMotor, b_escape_steps, Y_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!b_resolved) {
            b_resolved = resolve(&bMotor, -b_escape_steps, Y_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(b_resolved) {
            if(axis_correct) {
                Serial.println("Found B = Y");
                xMotor = &aMotor;
                yMotor = &bMotor;

                *y_direction_resolved = true;
            } else {
                Serial.println("Found B = X");
                xMotor = &bMotor;
                yMotor = &aMotor;

                *x_direction_resolved = true;
            }

            if(direction_correct) {
                Serial.println("Found + = +");
            } else {
                Serial.println("Found + = -");
                bMotor.set_inverted(true);
            }
        }
    } else {
        Serial.println("No switches are initially triggered.");
    }

    return (a_resolved | b_resolved);
}

void calibration(bool write_calibration) {
    Serial.println("Calibration beginning.");

    xMotor->set_direction(Motor::Forward);
    yMotor->set_direction(Motor::Forward);

    xMotor->set_speed(250);
    yMotor->set_speed(250);

    long x_distance = 0L;
    long y_distance = 0L;

    bool axes_resolved = false;
    bool x_direction_resolved = false;
    bool y_direction_resolved = false;

    axes_resolved = freedom(&x_direction_resolved, &y_direction_resolved);

    xMotor->set_speed(3500);
    yMotor->set_speed(3500);

    if(!axes_resolved) {
        Serial.println("Resolved nothing");
        Serial.println("Finding X");

        while(!any_limit()) {
            xMotor->move(-1);
        }

        if(y_limit()) {
            // Incorrect
            Motor *temp = xMotor;

            xMotor = yMotor;
            yMotor = temp;
        }
    }

    while(!(x_direction_resolved && y_direction_resolved)) {
        if(!x_direction_resolved) {
            if(!x_limit()) {
                xMotor->move(-1);
            } else {
                x_direction_resolved = true;

                if(x_pos_limit()) {
                    // Inverted
                    xMotor->set_inverted(true);
                }
            }
        }

        if(!y_direction_resolved) {
            if(!y_limit()) {
                yMotor->move(-1);
            } else {
                y_direction_resolved = true;

                if(y_pos_limit()) {
                    // Inverted
                    yMotor->set_inverted(true);
                }
            }
        }
    }

    Serial.println("Homing");

    while(!pos_limit()) {
        xMotor->move(1);
        yMotor->move(1);
    }

    while(!x_pos_limit()) {
        xMotor->move(1);
    }

    while(!y_pos_limit()) {
        yMotor->move(1);
    }

    while(!neg_limit()) {
        xMotor->move(-1);
        yMotor->move(-1);

        x_distance++;
        y_distance++;
    }

    while(!x_neg_limit()) {
        xMotor->move(-1);
        x_distance++;
    }

    while(!y_neg_limit()) {
        yMotor->move(-1);
        y_distance++;
    }

    Serial.println("Calibration procedure completed:");

    if(xMotor == &aMotor) {
        Serial.println("X = A, Y = B");
    } else {
        Serial.println("X = B, Y = A");
    }

    if(xMotor->is_inverted()) {
        Serial.println("X Inverted");
    }

    if(yMotor->is_inverted()) {
        Serial.println("Y Inverted");
    }

    Serial.println(x_distance, DEC);
    Serial.println(y_distance, DEC);

    if(write_calibration) {
        Serial.println("Writing calibration...");

        AxisSettings as;

        as.axis = Axis::X;

        if(xMotor == &aMotor) {
            as.motor = Motor::A;
        } else {
            as.motor = Motor::B;
        }

        as.flipped = xMotor->is_inverted();
        as.length = x_distance;

        write_axis_settings(Axis::X, &as);

        as.axis = Axis::Y;

        if(yMotor == &aMotor) {
            as.motor = Motor::A;
        } else {
            as.motor = Motor::B;
        }

        as.flipped = yMotor->is_inverted();
        as.length = y_distance;

        write_axis_settings(Axis::Y, &as);

        Serial.println("Calibration saved");
    }
}
