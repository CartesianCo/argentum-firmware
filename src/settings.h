
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Arduino.h"

/* Settings

PortX Direction
PortY Direction
PortX Axis
PortY Axis

Limit Switches NC/NO

(Calibrated Values)
Bed Width
Bed Depth

*/

#define PORT_X_DIR 0
#define PORT_Y_DIR 1
#define PORT_X_AXIS 2
#define PORT_Y_AXIS 3
#define LIMIT_NC 4
#define BED_WIDTH 5
#define BED_DEPTH 6

class Settings {
public:
    Settings();
    ~Settings();
};

typedef struct {
    uint8_t address;
    uint8_t value;
} setting_t;

setting_t readSetting(uint8_t settingID);

#endif
