#include "settings.h"

#include <EEPROM.h>
#include "utils.h"

void settings_format(void) {
    uint8_t dummy[32];

    memset(dummy, 0x00, 32);

    write_block(0, dummy, 32);
}

bool settings_integrity_check(void) {
    uint8_t size = 32;

    uint8_t checksum = 0x00;
    uint8_t correct = read_char(33);

    for(uint8_t i = 0; i < size; i++) {
        uint8_t value = read_char(i);

        checksum ^= value;
    }

    return (checksum == correct);
}

void write_axis_settings(const unsigned char axis, AxisSettings *settings) {
    uint8_t address = 0;

    if(axis == 'Y') {
        address = 10;
    }

    print_settings(settings);
    //write_block(address, settings, sizeof(AxisSettings));
}

void read_axis_settings(const unsigned char axis, AxisSettings *settings) {
    uint8_t address = 0;

    if(axis == 'Y') {
        address = 10;
    }

    read_block(address, settings, sizeof(AxisSettings));
}

uint8_t read_byte(uint8_t address) {
    return EEPROM.read(address);
}

void write_byte(uint8_t address, uint8_t value) {
    EEPROM.write(address, value);
}

void read_block(uint8_t address, void *buffer, uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        ((uint8_t *)buffer)[i] = read_byte(address + i);
    }
}

void write_block(uint8_t address, void *buffer, uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        write_byte((address + i), ((uint8_t *)buffer)[i]);
    }
}

bool read_bool(uint8_t address) {
    return (bool)read_byte(address);
}

void write_bool(uint8_t address, bool value) {
    write_byte(address, (uint8_t)value);
}

char read_char(uint8_t address) {
    return (char)read_byte(address);
}

void write_char(uint8_t address, char value) {
    write_byte(address, (uint8_t)value);
}

long read_long(uint8_t address) {
    long value = 0L;

    read_block(address, &value, 4);

    return value;
}

void write_long(uint8_t address, long value) {
    write_block(address, &value, 4);
}
