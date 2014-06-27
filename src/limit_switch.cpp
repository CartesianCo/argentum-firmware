#include "limit_switch.h"

uint8_t limit_switches(void) {
    uint8_t switches = 0b00000000;

    if(x_pos_limit()) {
        switches |= 0b00001000;
    }

    if(x_neg_limit()) {
        switches |= 0b00000100;
    }

    if(y_pos_limit()) {
        switches |= 0b00000010;
    }

    if(y_neg_limit()) {
        switches |= 0b00000001;
    }

    return switches;
}

void print_switch_status(uint8_t switches) {
    //Serial.print("Switch binary: ");
    //Serial.println(switches, BIN);

    if(X_POS(switches)) {
        Serial.print("X+ ");
    }

    if(X_NEG(switches)) {
        Serial.print("X- ");
    }

    if(Y_POS(switches)) {
        Serial.print("Y+ ");
    }

    if(Y_NEG(switches)) {
        Serial.print("Y- ");
    }

    Serial.print("\r\n");
}

void print_switch_status(void) {
    uint8_t switches = limit_switches();

    print_switch_status(switches);
}
