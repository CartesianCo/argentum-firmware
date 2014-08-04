#include "colour.h"

#include "../argentum/argentum.h"

// LED Control Functions
void colour_init(void) {
    pinMode(PIN_FET_RED, OUTPUT);
    pinMode(PIN_FET_GREEN, OUTPUT);
    pinMode(PIN_FET_BLUE, OUTPUT);

    colour(0);
}

void colour(uint32_t colour) {
    uint8_t red = RED_COMPONENT(colour);
    uint8_t green = GREEN_COMPONENT(colour);
    uint8_t blue = BLUE_COMPONENT(colour);

    colour_rgb(red, green, blue);
}

void colour_rgb(uint8_t red, uint8_t green, uint8_t blue) {
    colour_red(red);
    colour_green(green);
    colour_blue(blue);
}

void colour_red(uint8_t red) {
    if(red > 0) {
        //digitalWrite(RED_CHANNEL_PIN, HIGH);
        analogWrite(PIN_FET_RED, red);
    } else {
        digitalWrite(PIN_FET_RED, LOW);
    }
}

void colour_green(uint8_t green) {
    if(green > 0) {
        digitalWrite(PIN_FET_GREEN, HIGH);
    } else {
        digitalWrite(PIN_FET_GREEN, LOW);
    }
}

void colour_blue(uint8_t blue) {
    if(blue > 0) {
        digitalWrite(PIN_FET_BLUE, HIGH);
    } else {
        digitalWrite(PIN_FET_BLUE, LOW);
    }
}
