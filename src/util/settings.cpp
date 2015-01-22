#include "settings.h"

#include <EEPROM.h>
#include "utils.h"

PrinterSettings default_settings = {
    {
        {
            'B',
            false,
            14000L
        },
        {
            'A',
            true,
            10000L
        },
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
        Serial.println("Settings corrupt.");
        settings_print_settings(&global_settings);
    }

    return valid;
}

void settings_restore_defaults(void) {
    Serial.println("Restoring default settings.");
    settings_update_settings(&default_settings);
}

void settings_print_settings(PrinterSettings *settings) {
    settings_print_calibration(&(settings->calibration));

    uint8_t crc = settings_calculate_crc(settings);

    Serial.print("CRC: ");
    Serial.print(settings->crc, HEX);
    Serial.print(", Calculated: ");
    Serial.print(crc, HEX);

    if(crc == settings->crc) {
        Serial.println(" [GOOD]");
    } else {
        Serial.println(" [CORRUPT]");
    }
}

void settings_print_calibration(CalibrationData *calibration) {
    Serial.print("X axis: ");
    settings_print_axis_data(&(calibration->x_axis));

    Serial.print("Y axis: ");
    settings_print_axis_data(&(calibration->y_axis));
}

void settings_print_axis_data(AxisData *axis) {
    Serial.print((char)axis->motor);
    Serial.print(" motor, ");

    if(axis->flipped) {
        Serial.print("flipped, ");
    } else {
        Serial.print("not flipped, ");
    }

    Serial.print(axis->length);
    Serial.println(" steps");
}

void settings_print_axis_data_minimal(AxisData *axis) {
    Serial.print((char)axis->motor);
    /*Serial.print(", ");

    if(axis->flipped) {
        Serial.print("!, ");
    } else {
        Serial.print(" , ");
    }*/

    Serial.print(axis->length);
    //Serial.println("");
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
    read_block(SETTINGS_ADDRESS, settings, sizeof(PrinterSettings));
}

void settings_write_settings(PrinterSettings *settings) {
    // Make sure the CRC will be valid
    settings_update_crc();

    PrinterSettings existing_settings;
    settings_read_settings(&existing_settings);

    replace_block(SETTINGS_ADDRESS,
                  settings,
                  &existing_settings,
                  sizeof(PrinterSettings));
}


void settings_update_settings(PrinterSettings *settings) {
    memcpy(&global_settings, settings, sizeof(PrinterSettings));

    settings_update_crc();
}

void settings_update_calibration(CalibrationData *calibration) {
    memcpy(&(global_settings.calibration),
           calibration,
           sizeof(CalibrationData));

    settings_update_crc();
}

void settings_update_x_data(AxisData *axis_data) {
    memcpy(&(global_settings.calibration.x_axis), axis_data, sizeof(AxisData));

    settings_update_crc();
}

void settings_update_y_data(AxisData *axis_data) {
    memcpy(&(global_settings.calibration.y_axis), axis_data, sizeof(AxisData));

    settings_update_crc();
}

void settings_update_crc(void) {
    global_settings.crc = settings_calculate_crc(&global_settings);
}

// Base EEPROM Functions

uint8_t read_byte(const uint16_t address) {
    return EEPROM.read(address);
}

void write_byte(const uint16_t address, const uint8_t value, bool reduce_wear) {
    if(reduce_wear) {
        uint8_t existing = read_byte(address);

        if(value == existing) {
            return;
        }
    }

    EEPROM.write(address, value);
}

void read_block(const uint16_t address, void *buffer, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        ((uint8_t *)buffer)[i] = read_byte(address + i);
    }
}

void write_block(const uint16_t address, void *buffer, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        write_byte((address + i), ((uint8_t *)buffer)[i], true);
    }
}

// This function is to reduce write wear on the EEPROM
void replace_block(const uint16_t address, void *buffer, void *existing, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        if(((uint8_t *)buffer)[i] != ((uint8_t *)existing)[i]) {
            write_byte((address + i), ((uint8_t *)buffer)[i], false);
        }
    }
}
