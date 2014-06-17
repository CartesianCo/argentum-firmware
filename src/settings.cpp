#include "settings.h"

#include <EEPROM.h>

setting_t readSetting(uint8_t settingID) {
    setting_t setting;

    setting.address = settingID;
    setting.value = EEPROM.read(settingID);

    return setting;
}
