#include "settings.h"

#include <EEPROM.h>

Settings::Settings() {
}

Settings::~Settings() {
}

void Settings::write_axis_settings(const unsigned char axis, AxisSettings *settings) {
    uint8_t address = 0;

    if(axis == 'Y') {
        address = 10;
    }

    this->write_block(address, settings, sizeof(AxisSettings));
}

void Settings::read_axis_settings(const unsigned char axis, AxisSettings *settings) {
    uint8_t address = 0;

    if(axis == 'Y') {
        address = 10;
    }

    this->read_block(address, settings, sizeof(AxisSettings));
}

uint8_t Settings::read_byte(uint8_t address) {
    return EEPROM.read(address);
}

void Settings::write_byte(uint8_t address, uint8_t value) {
    EEPROM.write(address, value);
}

void Settings::read_block(uint8_t address, void *buffer, uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        ((uint8_t *)buffer)[i] = this->read_byte(address + i);
    }
}

void Settings::write_block(uint8_t address, void *buffer, uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        this->write_byte((address + i), ((uint8_t *)buffer)[i]);
    }
}

bool Settings::read_bool(uint8_t address) {
    return (bool)this->read_byte(address);
}

void Settings::write_bool(uint8_t address, bool value) {
    this->write_byte(address, (uint8_t)value);
}

char Settings::read_char(uint8_t address) {
    return (char)this->read_byte(address);
}

void Settings::write_char(uint8_t address, char value) {
    this->write_byte(address, (uint8_t)value);
}

long Settings::read_long(uint8_t address) {
    long value = 0L;

    this->read_block(address, &value, 4);

    return value;
}

void Settings::write_long(uint8_t address, long value) {
    this->write_block(address, &value, 4);
}
