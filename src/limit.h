#ifndef _LIMIT_H_
#define _LIMIT_H_

#include <stdint.h>


#define X_POS_BIT 0b00001000
#define X_NEG_BIT 0b00000100
#define Y_POS_BIT 0b00000010
#define Y_NEG_BIT 0b00000001

#define X_MASK    0b00001100
#define Y_MASK    0b00000011
#define POS_MASK  0b00001010
#define NEG_MASK  0b00000101

namespace Limit {
    uint8_t switches(void);

    inline bool x_positive(void);
    inline bool x_negative(void);
    inline bool y_positive(void);
    inline bool y_negative(void);

    inline bool x(void);
    inline bool y(void);

    inline bool positive(void);
    inline bool negative(void);

    inline bool any(void);
}

#endif
