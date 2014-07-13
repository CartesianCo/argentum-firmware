#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

#define RAM_TOTAL 8192

template<class T> inline Print &operator <<(Print &obj, T arg) {
    obj.print(arg);
    return obj;
}

int ram_free(void);
int ram_used(void);
double ram_utilisation(void);

uint8_t CRC8(const void *buffer, const uint16_t length);
uint8_t CRC8_add_byte(uint8_t data, uint8_t seed_crc);

#endif
