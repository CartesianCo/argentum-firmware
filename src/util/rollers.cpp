#include "rollers.h"
#include <Servo.h>

#include "utils.h"

Rollers::Rollers() {
    //TODO: This causes the servo to not take any updates (it still holds position)
    // I'm guessing it's something to do with initialisation orders since this
    // object is typically created as a global.

    //enable();
}

Rollers::~Rollers() {
    disable();
}

void Rollers::enable(void) {
    roller_servo.attach(roller_servo_pin);
    retract();
}

void Rollers::disable(void) {
    roller_servo.detach();
}

void Rollers::deploy(void) {
    deployed = true;
    roller_servo.write(deployed_position);
}

void Rollers::retract(void) {
    deployed = false;
    roller_servo.write(retracted_position);
}

void Rollers::angle(unsigned char angle) {
    if(angle > 180) {
        return;
    }

    roller_servo.write(angle);
}

unsigned int Rollers::width_with_overlap(double overlap) {
    return (roller_width - (roller_width * overlap));
}
