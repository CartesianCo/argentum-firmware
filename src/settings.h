
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Arduino.h"

/* Settings

A Axis (char)
A Flipped (bool)
A Length (long)

B Axis (char)
B Flipped (bool)
B Length (long)

Future Support:
Limit Switches NC/NO (bool)

*/

struct AxisSettings {
    uint8_t axis;
    uint8_t motor;
    bool flipped;
    long length;
};

void settings_integrity_check(void);

void write_axis_settings(const unsigned char axis, AxisSettings *settings);
void read_axis_settings(const unsigned char axis, AxisSettings *settings);

uint8_t read_byte(uint8_t address);
void write_byte(uint8_t address, uint8_t value);

void read_block(uint8_t address, void *buffer, uint8_t length);
void write_block(uint8_t address, void *buffer, uint8_t length);

bool read_bool(uint8_t address);
void write_bool(uint8_t address, bool value);

char read_char(uint8_t address);
void write_char(uint8_t address, char value);

long read_long(uint8_t address);
void write_long(uint8_t address, long value);

uint8_t read_setting(uint8_t id);
void write_setting(uint8_t id, uint8_t value);

#endif
