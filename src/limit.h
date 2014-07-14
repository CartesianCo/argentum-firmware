#ifndef _LIMIT_H_
#define _LIMIT_H_

#include <stdint.h>

#define X_POS_HARDWARE_BIT 0b00001000
#define X_NEG_HARDWARE_BIT 0b00000001
#define Y_POS_HARDWARE_BIT 0b00000010
#define Y_NEG_HARDWARE_BIT 0b00001000

#define X_POS_BIT 0b00001000
#define X_NEG_BIT 0b00000100
#define Y_POS_BIT 0b00000010
#define Y_NEG_BIT 0b00000001

#define X_MASK    0b00001100
#define Y_MASK    0b00000011
#define POS_MASK  0b00001010
#define NEG_MASK  0b00000101

/*
class LimitSwitch {
public:
    LimitSwitch(volatile uint8_t *port, uint8_t bit);
    ~LimitSwitch();

    bool triggered(void);

private:
    volatile uint8_t *port;
    uint8_t bit;
};
*/

uint8_t limit_switches(void);

bool limit_x_positive(void);
bool limit_x_negative(void);
bool limit_y_positive(void);
bool limit_y_negative(void);

bool limit_x(void);
bool limit_y(void);

bool limit_positive(void);
bool limit_negative(void);

bool limit_any(void);

#endif
