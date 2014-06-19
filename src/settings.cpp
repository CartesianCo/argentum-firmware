#include "settings.h"

#include <EEPROM.h>

uint8_t read_setting(uint8_t setting_id) {
    setting_t setting;

    setting.address = setting_id;
    setting.value = EEPROM.read(setting_id);

    return setting.value;
}
