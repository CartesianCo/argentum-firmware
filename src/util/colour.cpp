#include "colour.h"

#include "../argentum/argentum.h"
#include "utils.h"

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
    fet_set_value(PIN_FET_RED, red);
}

void colour_green(uint8_t green) {
    fet_set_value(PIN_FET_GREEN, green);
}

void colour_blue(uint8_t blue) {
    fet_set_value(PIN_FET_BLUE, blue);
}
