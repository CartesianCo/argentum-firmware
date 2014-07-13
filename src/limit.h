#ifndef _LIMIT_H_
#define _LIMIT_H_

#include <stdint.h>

class Limit {
public:
    Limit();
    ~Limit();

    static inline uint8_t switches(void);

    static inline bool any_limit(void);
};

#endif
