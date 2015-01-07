#include "rollers.h"
#include "utils.h"
#include "../util/logging.h"

Rollers::Rollers() {
    //TODO: This causes the servo to not take any updates (it still holds position)
    // I'm guessing it's something to do with initialisation orders since this
    // object is typically created as a global.

    retracted_position = 70;
    deployed_position = 80;
    //enable();
}

Rollers::~Rollers() {
    disable();
}

void Rollers::enable(void) {
    roller_servo.attach(ROLLER_SERVO_PIN);
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

unsigned char Rollers::getangle(void)
{
    return roller_servo.read();
}

void Rollers::angle(unsigned char angle) {
    // logger.info("Changing servo angle.");
    if(angle >= 0 && angle <= 180)
    {
      roller_servo.write(angle);
      logger.info("Servo angle changed to:");
      logger.info(angle);
    }
    else
      logger.info("Invalide servo angle.");
}

// Set the retract position to angle.
void Rollers::setrp(unsigned char angle)
{
    if(angle >= 0 && angle <= 180)
      retracted_position = angle;
    else
      logger.info("Invalide servo angle.");
}

// Set the deploy position to angle.
void Rollers::setdp(unsigned char angle)
{
    if(angle >= 0 && angle <= 180)
    	deployed_position = angle;
    else
      logger.info("Invalide servo angle.");
}

unsigned int Rollers::width_with_overlap(double overlap) {
    return (roller_width - (roller_width * overlap));
}
