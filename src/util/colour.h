/*

LEDStrip.h

Created by Michael Shiel, 140617

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
