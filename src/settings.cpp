#include "settings.h"

#include <EEPROM.h>
#include "utils.h"

PrinterSettings default_settings = {
    {
        'X',
        'B',
        false,
        14000L
    },
    {
        'Y',
        'A',
        true,
        10000L
    },
    0xFA
};

PrinterSettings global_settings;

// Settings helpers

bool settings_initialise(bool correct) {
    settings_read_settings(&global_settings);

    bool valid = settings_integrity_check(&global_settings);

    if(!valid && correct) {
        settings_restore_defaults();
    }

    if(!valid) {
        settings_print_settings(&global_settings);
    }

    return valid;
}

void settings_restore_defaults(void) {
    memcpy(&global_settings, &default_settings, sizeof(PrinterSettings));

    settings_write_settings(&global_settings);
}

void settings_print_settings(PrinterSettings *settings) {
    settings_print_axis_settings(&(settings->x_axis));
    settings_print_axis_settings(&(settings->y_axis));

    uint8_t crc = settings_calculate_crc(settings);

    Serial.print("CRC: ");
    Serial.print(settings->crc, HEX);
    Serial.print(", Calculated: ");
    Serial.print(crc, HEX);

    if(crc == settings->crc) {
        Serial.println(" [GOOD \xFB]");
    } else {
        Serial.println(" [CORRUPT \xB0]");
    }
}

void settings_print_axis_settings(AxisSettings *settings) {
    Serial.print((char)settings->axis);
    Serial.print(" axis -> ");
    Serial.print((char)settings->motor);
    Serial.print(" motor, ");

    if(settings->flipped) {
        Serial.print(" flipped, ");
    } else {
        Serial.print(" not flipped, ");
    }

    Serial.print(settings->length);
    Serial.println(" steps");
}

// Settings CRC Utilities

uint8_t settings_calculate_crc(PrinterSettings *settings) {
    // We want to calculate the checksum of all the data except the checksum
    // value itself, which is a uint16_t at the end of the struct.
    uint16_t struct_size = sizeof(PrinterSettings) - sizeof(uint8_t);

    return CRC8(settings, struct_size);
}

bool settings_integrity_check(PrinterSettings *settings) {
    uint8_t crc = settings_calculate_crc(settings);

    return (crc == settings->crc);
}

// Settings Read and Write

void settings_read_settings(PrinterSettings *settings) {
    read_block(SETTINGS_ADDRESS, &global_settings, sizeof(PrinterSettings));
}

void settings_write_settings(PrinterSettings *settings) {
    write_block(SETTINGS_ADDRESS, &global_settings, sizeof(PrinterSettings));
}

// Base EEPROM Functions

void read_block(const uint16_t address, void *buffer, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        ((uint8_t *)buffer)[i] = read_byte(address + i);
    }
}

void write_block(const uint16_t address, void *buffer, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        write_byte((address + i), ((uint8_t *)buffer)[i]);
    }
}

uint8_t read_byte(const uint16_t address) {
    //return EEPROM.read(address);
    uint8_t value = EEPROM.read(address);

    Serial.print('W');
    Serial.print(value, HEX);
    Serial.print('|');

    return value;
}

void write_byte(const uint16_t address, const uint8_t value) {
    //EEPROM.write(address, value);
    Serial.print('R');
    Serial.print(value, HEX);
    Serial.print('|');
}
