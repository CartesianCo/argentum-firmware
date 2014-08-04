#include "utils.h"

#include "settings.h"
#include "logging.h"
#include "limit.h"

#include "../argentum/argentum.h"

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

    LoggerWrapper &info = logger.info() << "Limits: ";

    if(switches == 0b00000000) {
        info << "None triggered.";
    }

    if(X_POS(switches)) {
        info << "X+ ";
    }

    if(X_NEG(switches)) {
        info << "X- ";
    }

    if(Y_POS(switches)) {
        info << "Y+ ";
    }

    if(Y_NEG(switches)) {
        info << "Y- ";
    }

    info << Comms::endl;

    if((X_POS(switches) && X_NEG(switches))
        || (Y_POS(switches) && Y_NEG(switches))) {
            logger.info("Impossible configuration on limit switches, inverting.");

            limit_switch_nc = !limit_switch_nc;
    }
}

void print_switch_status(void) {
    uint8_t switches = limit_switches();

    print_switch_status(switches);
}

void fet_initialise(void) {
    // Disable FETs by default
    pinMode(FET_1_PIN, OUTPUT);
    pinMode(FET_2_PIN, OUTPUT);
    pinMode(FET_3_PIN, OUTPUT);

    fet_set_value(FET_1_PIN, 0);
    fet_set_value(FET_2_PIN, 0);
    fet_set_value(FET_3_PIN, 0);
}

void fet_set_value(uint8_t fet, uint8_t value) {
    analogWrite(fet, value);
}

void analog_initialise(void) {
    // General Analog Inputs
    pinMode(PIN_ANALOG_1, INPUT);
    pinMode(PIN_ANALOG_2, INPUT);
    pinMode(PIN_ANALOG_3, INPUT);

    // Voltage Feedback (9V Sense)
    pinMode(PIN_PRIMITIVE_VOLTAGE, INPUT);
}

uint16_t analog_read(uint8_t analog) {
    return analogRead(analog);
}

double primitive_voltage(void) {
    uint16_t adc_reading = analog_read(PIN_PRIMITIVE_VOLTAGE);

    // Hardware voltage divider of 1/3
    double voltage = (adc_reading / 1024.0) * 5.0 * 3.0;

    return voltage;
}
