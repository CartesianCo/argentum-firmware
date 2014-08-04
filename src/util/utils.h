#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

#define RAM_TOTAL 8192

int ram_free(void);
int ram_used(void);
double ram_utilisation(void);

uint8_t CRC8(const void *buffer, const uint16_t length);
uint8_t CRC8_add_byte(uint8_t data, uint8_t seed_crc);

void print_switch_status(void);
void print_switch_status(uint8_t switches);

void fet_initialise(void);
void fet_set_value(uint8_t fet, uint8_t value);

void analog_initialise(void);
uint16_t analog_read(uint8_t analog);

#endif
