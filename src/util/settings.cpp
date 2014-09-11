#include "settings.h"

#include <EEPROM.h>
#include "utils.h"
#include "logging.h"

#include "../argentum/argentum.h"

PrinterSettings default_settings = {
    0x01,
    {
        {
            'A',
            false,
            14000L
        },
        {
            'B',
            false,
            10000L
        },
    },
    {
        0,
        0
    },
    0xFA
};

PrinterSettings global_settings;

// Settings helpers

bool settings_initialise(bool correct) {
    settings_read_settings(&global_settings);

    bool valid = settings_integrity_check(&global_settings);

    if(!valid) {
        logger.warn("Settings corrupt.");
        settings_print_settings(&global_settings);

        if(correct) {
            settings_restore_defaults();

            logger.warn("Restored to defaults.");
        }
    }

    return valid;
}

void settings_restore_defaults(void) {
    logger.info("Restoring default settings.");
    settings_update_settings(&default_settings);
    settings_write_settings(&global_settings);
}

void settings_print_settings(PrinterSettings *settings) {
    settings_print_calibration(&(settings->stepper_calibration));

    uint8_t crc = settings_calculate_crc(settings);

    logger.info() << "CRC: " << settings->crc << ", Calculated: " << crc
            << Comms::endl;

    if(crc == settings->crc) {
        logger.info(" [GOOD]");
    } else {
        logger.warn(" [CORRUPT]");
    }
}

void settings_print_calibration(StepperCalibrationData *calibration) {
    logger.info("X axis: ");
    settings_print_axis_data(&(calibration->x_axis));

    logger.info("Y axis: ");
    settings_print_axis_data(&(calibration->y_axis));
}

void settings_print_axis_data(AxisData *axis) {
    logger.info() << (char)axis->motor << " motor, " << axis->flipped
            << ", " << axis->length << " steps." << Comms::endl;
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

    return (crc && (crc == settings->crc));
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

    load_settings();
}

void settings_update_calibration(StepperCalibrationData *calibration) {
    memcpy(&(global_settings.stepper_calibration),
           calibration,
           sizeof(StepperCalibrationData));

    settings_update_crc();
}

void settings_update_x_data(AxisData *axis_data) {
    memcpy(&(global_settings.stepper_calibration.x_axis), axis_data, sizeof(AxisData));

    settings_update_crc();
}

void settings_update_y_data(AxisData *axis_data) {
    memcpy(&(global_settings.stepper_calibration.y_axis), axis_data, sizeof(AxisData));

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
