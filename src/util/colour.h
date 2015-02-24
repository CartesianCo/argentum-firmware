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

#ifndef _COLOUR_H_
#define _COLOUR_H_

#include <Arduino.h>

#define COLOUR_NONE     0x00000000
#define COLOUR_HOME 	0x000000FF // Blue
#define COLOUR_PRINTING 0x00FF0000 // Red
#define COLOUR_FINISHED 0x0000FF00 // Green

#define RED_COMPONENT(x)    ((x & 0x00FF0000) >> 16);
#define GREEN_COMPONENT(x)  ((x & 0x0000FF00) >> 8);
#define BLUE_COMPONENT(x)   (x & 0x000000FF);

// LED Control Functions
void colour_init(void);
void colour(uint32_t colour);
void colour_rgb(uint8_t red, uint8_t green, uint8_t blue);
void colour_red(uint8_t red);
void colour_green(uint8_t green);
void colour_blue(uint8_t blue);

#endif
