#ifndef _BOARDTESTS_H_
#define _BOARDTESTS_H_

#include <stdint.h>

enum TestStages {
    STAGE_0 = 0,
    STAGE_1,
    STAGE_2,
    STAGE_3,
    STAGE_4,
    STAGE_5,
    STAGE_6,
    STAGE_7
};

extern uint8_t current_stage;

void run_tests(void);

#endif
