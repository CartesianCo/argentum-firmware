#include "limit.h"
#include <Arduino.h>

uint8_t Limit::switches(void) {
    uint8_t switches = 0b00000000;

    if(x_positive()) {
        switches |= X_POS_BIT;
    }

    if(x_negative()) {
        switches |= X_NEG_BIT;
    }

    if(y_positive()) {
        switches |= Y_POS_BIT;
    }

    if(y_negative()) {
        switches |= Y_NEG_BIT;
    }

    return switches;
}

bool Limit::x_positive(void) {
    return (PINE & 0b00001000);
}

bool Limit::x_negative(void) {
    return (PINF & 0b00000001);
}

bool Limit::y_positive(void) {
    return (PINF & 0b00000010);
}

bool Limit::y_negative(void) {
    return (PINH & 0b00001000);
}

bool Limit::x(void) {
    return (x_positive() || x_negative());
}

bool Limit::y(void) {
    return (y_positive() || y_negative());
}

bool Limit::positive(void) {
    return (x_positive() || y_positive());
}

bool Limit::negative(void) {
    return (x_negative() || y_negative());
}

bool Limit::any(void) {
    return (x() || y());
}
