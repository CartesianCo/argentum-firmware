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

#ifndef _ROLLERS_H_
#define _ROLLERS_H_

#include <Servo.h>
#define ROLLER_SERVO_PIN      10	// AOUT 1

class Rollers {
public:
    Rollers();
    ~Rollers();

    void enable(void);
    void disable(void);

    void deploy(void);
    void retract(void);

    unsigned char getangle(void);
    void angle(unsigned char angle);
    void setrp(unsigned char angle);
    void setdp(unsigned char angle);

    static unsigned int width_with_overlap(double overlap);

    static const int roller_width = 1280;
    static const int roller_spacing = 500;

private:
    bool deployed;

    Servo roller_servo;
};

#endif
