/*

LEDStrip.h

Created by Michael Shiel, 140617

*/

#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include "Arduino.h"

#define RED_CHANNEL_PIN 3
#define GREEN_CHANNEL_PIN 2
#define BLUE_CHANNEL_PIN 4

#define COLOUR_NONE		0x00000000
#define COLOUR_HOME 	0x000000FF
#define COLOUR_PRINTING 0x00FF0000
#define COLOUR_FINISHED 0x0000FF00

#define RED_COMPONENT(x) ((x & 0x00FF0000) >> 16);
#define GREEN_COMPONENT(x) ((x & 0x0000FF00) >> 8);
#define BLUE_COMPONENT(x) (x & 0x000000FF);


// LED Control Functions
void initLED(void);
void setLEDToColour(uint32_t colour);
void setLED(uint8_t red, uint8_t green, uint8_t blue);
void setRed(uint8_t red);
void setGreen(uint8_t green);
void setBlue(uint8_t blue);

#endif
