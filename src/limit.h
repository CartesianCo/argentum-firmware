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

class LimitSwitch {
public:
    LimitSwitch(volatile uint8_t *port, uint8_t bit);
    ~LimitSwitch();

    bool triggered(void);

private:
    volatile uint8_t *port;
    uint8_t bit;
};

namespace Limit {
    uint8_t switches(void);

    bool x_positive(void);
    bool x_negative(void);
    bool y_positive(void);
    bool y_negative(void);

    bool x(void);
    bool y(void);

    bool positive(void);
    bool negative(void);

    bool any(void);
}

extern LimitSwitch x_positive;

#endif
