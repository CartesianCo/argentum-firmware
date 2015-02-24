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

#include "rollers.h"
#include "utils.h"
#include "../util/logging.h"
#include "settings.h"

Rollers::Rollers() {
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
    roller_servo.write(global_settings.rollerOptions.deployed_pos);
}

void Rollers::retract(void) {
    deployed = false;
    roller_servo.write(global_settings.rollerOptions.retracted_pos);
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
      global_settings.rollerOptions.retracted_pos = angle;
    else
      logger.info("Invalide servo angle.");
}

// Set the deploy position to angle.
void Rollers::setdp(unsigned char angle)
{
    if(angle >= 0 && angle <= 180)
        global_settings.rollerOptions.deployed_pos = angle;
    else
      logger.info("Invalide servo angle.");
}

unsigned int Rollers::width_with_overlap(double overlap) {
    return (roller_width - (roller_width * overlap));
}
