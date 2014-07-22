#include "LEDStrip.h"

// LED Control Functions
void initLED(void) {
    pinMode(RED_CHANNEL_PIN, OUTPUT);
    pinMode(GREEN_CHANNEL_PIN, OUTPUT);
    pinMode(BLUE_CHANNEL_PIN, OUTPUT);
}

void setLEDToColour(uint32_t colour) {
	uint8_t red = RED_COMPONENT(colour);
	uint8_t green = GREEN_COMPONENT(colour);
	uint8_t blue = BLUE_COMPONENT(colour);

	setLED(red, green, blue);
}

void setLED(uint8_t red, uint8_t green, uint8_t blue) {
	setRed(red);
	setGreen(green);
	setBlue(blue);
}

void setRed(uint8_t red) {
	if(red > 0) {
		//digitalWrite(RED_CHANNEL_PIN, HIGH);
        analogWrite(RED_CHANNEL_PIN, red);
	} else {
		digitalWrite(RED_CHANNEL_PIN, LOW);
	}
}

void setGreen(uint8_t green) {
	if(green > 0) {
		digitalWrite(GREEN_CHANNEL_PIN, HIGH);
	} else {
		digitalWrite(GREEN_CHANNEL_PIN, LOW);
	}
}

void setBlue(uint8_t blue) {
	if(blue > 0) {
		digitalWrite(BLUE_CHANNEL_PIN, HIGH);
	} else {
		digitalWrite(BLUE_CHANNEL_PIN, LOW);
	}
}
