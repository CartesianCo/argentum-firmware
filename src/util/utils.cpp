#include "utils.h"

#include "settings.h"
#include "limit.h"

int ram_used(void) {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int ram_free(void) {
    return RAM_TOTAL - ram_used();
}

double ram_utilisation(void) {
    return (ram_used() / (RAM_TOTAL / 100.0)); // 8192 bytes total * 100 to be percent.
}

uint8_t CRC8(const void *buffer, const uint16_t length) {
    uint8_t crc = 0x00;

    for(uint16_t i = 0; i < length; i++) {
        uint8_t data = ((uint8_t *)buffer)[i];

        crc = CRC8_add_byte(data, crc);
    }

    return crc;
}

uint8_t CRC8_add_byte(uint8_t data, uint8_t crc) {
    for(uint8_t i = 8; i > 0; i--) {
        uint8_t sum = (crc ^ data) & 0x01;

        crc = crc >> 1;

        if (sum) {
            crc ^= 0x8C;
        }

        data = data >> 1;
    }

    return crc;
}

void print_switch_status(uint8_t switches) {
    //Serial.print("Switch binary: ");
    //Serial.println(switches, BIN);
    if(switches == 0b00000000) {
        Serial.print("None triggered.");
    }

    if(X_POS(switches)) {
        Serial.print("X+ ");
    }

    if(X_NEG(switches)) {
        Serial.print("X- ");
    }

    if(Y_POS(switches)) {
        Serial.print("Y+ ");
    }

    if(Y_NEG(switches)) {
        Serial.print("Y- ");
    }

    Serial.print("\r\n");
}

void print_switch_status(void) {
    uint8_t switches = limit_switches();

    print_switch_status(switches);
}
