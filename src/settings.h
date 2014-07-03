
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

// Insist that our structs are 1-byte boundary aligned. (No padding)
#pragma pack(push, 1)

struct AxisSettings {
    uint8_t axis;
    uint8_t motor;
    bool flipped;
    long length;
};

struct PrinterSettings {
    AxisSettings x_axis;
    AxisSettings y_axis;
    uint8_t checksum;
};

#pragma pack(pop)

extern PrinterSettings default_settings;

bool settings_integrity_check(void);
uint16_t settings_calculate_checksum(PrinterSettings *settings);

//void write_axis_settings(const unsigned char axis, AxisSettings *settings);
//void read_axis_settings(const unsigned char axis, AxisSettings *settings);

uint8_t read_byte(uint8_t address);
void write_byte(uint8_t address, uint8_t value);

void read_block(uint8_t address, void *buffer, uint8_t length);
void write_block(uint8_t address, void *buffer, uint8_t length);

/*
bool read_bool(uint8_t address);
void write_bool(uint8_t address, bool value);

char read_char(uint8_t address);
void write_char(uint8_t address, char value);

long read_long(uint8_t address);
void write_long(uint8_t address, long value);

uint8_t read_setting(uint8_t id);
void write_setting(uint8_t id, uint8_t value);
*/

#endif
