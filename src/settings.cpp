#include "settings.h"

#include <EEPROM.h>

uint8_t read_setting(uint8_t id) {
    return EEPROM.read(id);
}

void write_setting(uint8_t id, uint8_t value) {
    EEPROM.write(id, value);
}
