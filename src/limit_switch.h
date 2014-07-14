#ifndef _LIMIT_SWITCH_H_
#define _LIMIT_SWITCH_H_

#include "Arduino.h"

#define LIMITS_NC

#define X_POS_BIT 0b00001000
#define X_NEG_BIT 0b00000100
#define Y_POS_BIT 0b00000010
#define Y_NEG_BIT 0b00000001

#define X_MASK    0b00001100
#define Y_MASK    0b00000011
#define POS_MASK  0b00001010
#define NEG_MASK  0b00000101

#define X_POS(switches) (switches & X_POS_BIT)
#define X_NEG(switches) (switches & X_NEG_BIT)
#define Y_POS(switches) (switches & Y_POS_BIT)
#define Y_NEG(switches) (switches & Y_NEG_BIT)

#define X_LIMIT(switches) (switches & X_MASK)
#define Y_LIMIT(switches) (switches & Y_MASK)

#define POS_LIMIT(switches) (switches & POS_MASK)
#define NEG_LIMIT(switches) (switches & NEG_MASK)

#ifndef LIMITS_NC
    #define x_pos_limit() (PINE & 0b00001000)
    #define x_neg_limit() (PINF & 0b00000001)
    #define y_pos_limit() (PINF & 0b00000010)
    #define y_neg_limit() (PINH & 0b00001000)
#else
    #define x_pos_limit() !(PINE & 0b00001000)
    #define x_neg_limit() !(PINF & 0b00000001)
    #define y_pos_limit() !(PINF & 0b00000010)
    #define y_neg_limit() !(PINH & 0b00001000)
#endif

#define pos_limit() (x_pos_limit() || y_pos_limit())
#define neg_limit() (x_neg_limit() || y_neg_limit())

#define x_limit() (x_pos_limit() || x_neg_limit())
#define y_limit() (y_pos_limit() || y_neg_limit())
#define any_limit() (x_limit() || y_limit())

/*

Pinout Reference: http://arduino.cc/en/uploads/Hacking/PinMap2560big.png

X Positive Limit -> 5  (Port E, Bit 3)
X Negative Limit -> A0 (Port F, Bit 0)
Y Positive Limit -> A1 (Port F, Bit 1)
Y Negative Limit -> 6  (Port H, Bit 3)

*/

const int x_pos_limit_pin = 5;
const int x_neg_limit_pin = A0;
const int y_pos_limit_pin = A1;
const int y_neg_limit_pin = 6;

uint8_t __limit_switches(void);

void print_switch_status(void);
void print_switch_status(uint8_t switches);

#endif
