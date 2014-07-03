
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

const uint16_t SETTINGS_ADDRESS = 0;

// Insist that our structs are 1-byte boundary aligned. (No padding)
#pragma pack(push, 1)

struct AxisSettings {
    uint8_t axis;
    uint8_t motor;
    bool flipped;
    uint16_t length;
};

struct PrinterSettings {
    AxisSettings x_axis;
    AxisSettings y_axis;
    uint8_t crc;
};

#pragma pack(pop)

extern PrinterSettings default_settings;
extern PrinterSettings global_settings;

bool settings_initialise(bool correct);
void settings_restore_defaults(void);

void settings_print_settings(PrinterSettings *settings);
void settings_print_axis_settings(AxisSettings *settings);

uint8_t settings_calculate_crc(PrinterSettings *settings);
bool settings_integrity_check(PrinterSettings *settings);

void settings_read_settings(PrinterSettings *settings);
void settings_write_settings(PrinterSettings *settings);

uint8_t read_byte(uint16_t address);
void write_byte(uint16_t address, uint8_t value);

void read_block(uint16_t address, void *buffer, uint16_t length);
void write_block(uint16_t address, void *buffer, uint16_t length);


#endif
