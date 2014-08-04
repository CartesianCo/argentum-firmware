#include "boardtests.h"

#include <Servo.h>

#include "argentum.h"
#include "../util/limit.h"
#include "../util/utils.h"
#include "../util/logging.h"
#include "../util/colour.h"
#include "../util/SdFat/SdFat.h"

uint8_t current_stage = 0;

#define check_advance() if(_check_advance()) break;

bool _check_advance(void) {
    if(Serial.available()) {
        uint8_t c = Serial.read();

        if(c == '\r' || c == '\n') {
            while(Serial.available()) {
                Serial.read();
            }

            if(current_stage < STAGE_7) {
                current_stage++;
            } else {
                current_stage = STAGE_0;
            }

            logger.info() << "Advancing to stage " << current_stage
                << Comms::endl;

            return true;
        }
    }

    return false;
}

void stage_0(void) {
    logger.info("Stage 0 - Check Hardware.");

    while(current_stage == STAGE_0) {
        check_advance();
    }
}

void stage_1(void) {
    logger.info("Stage 1 - Read SD card, report over serial.");

    if(!sd.begin(53, SPI_HALF_SPEED)) {
        logger.error("Failed to initialise SD card.");
    } else {
        logger.info("Files on SD card:");

        SdFile file;
        char name[13];

        sd.vwd()->rewind();

        while (file.openNext(sd.vwd(), O_READ)) {
            file.getFilename(name);

            logger.info(name);

            file.close();
        }
    }

    while(current_stage == STAGE_1) {
        check_advance();
    }
}

void stage_2(void) {
    logger.info("Stage 2 - Feedback Vadj voltage over serial.");

    analog_initialise();

    while(current_stage == STAGE_2) {
        check_advance();

        uint16_t reading = analog_read(PIN_PRIMITIVE_VOLTAGE);
        double voltage = primitive_voltage();

        logger.info() << "Voltage = " << voltage << " V" << " (" << reading
            << ")"<< Comms::endl;

        delay(100);
    }
}

void fet_ramp(uint8_t pin) {
    for(uint8_t i = 0; i < 255; i++) {
        fet_set_value(pin, i);

        delay(4);
        check_advance();
    }

    for(uint8_t i = 255; i > 0; i--) {
        fet_set_value(pin, i);

        delay(4);
        check_advance();
    }

    fet_set_value(pin, 0);
}

void stage_3(void) {
    logger.info("Stage 3 - Blink LED strips in series.");

    fet_initialise();

    while(current_stage == STAGE_3) {
        logger.info("Ramping Fet 1");
        fet_ramp(PIN_FET_1);

        if(current_stage != STAGE_3) {
            break;
        }

        logger.info("Ramping Fet 2");
        fet_ramp(PIN_FET_2);

        if(current_stage != STAGE_3) {
            break;
        }

        logger.info("Ramping Fet 3");
        fet_ramp(PIN_FET_3);

        if(current_stage != STAGE_3) {
            break;
        }

        logger.info("All off - 2 seconds");
        fet_set_value(PIN_FET_1, 0);
        fet_set_value(PIN_FET_2, 0);
        fet_set_value(PIN_FET_3, 0);
        delay(2000);
    }

    fet_set_value(PIN_FET_1, 0);
    fet_set_value(PIN_FET_2, 0);
    fet_set_value(PIN_FET_3, 0);
}

void stage_4(void) {
    logger.info("Stage 4 - Blink RGB channels in series");

    colour_init();

    while(current_stage == STAGE_4) {
        logger.info("Ramping Red");
        fet_ramp(PIN_FET_RED);

        if(current_stage != STAGE_4) {
            break;
        }

        logger.info("Ramping Green");
        fet_ramp(PIN_FET_GREEN);

        if(current_stage != STAGE_4) {
            break;
        }

        logger.info("Ramping Blue");
        fet_ramp(PIN_FET_BLUE);

        if(current_stage != STAGE_4) {
            break;
        }

        logger.info("All off - 2 seconds");
        fet_set_value(PIN_FET_RED, 0);
        fet_set_value(PIN_FET_GREEN, 0);
        fet_set_value(PIN_FET_BLUE, 0);
        delay(2000);
    }

    fet_set_value(PIN_FET_RED, 0);
    fet_set_value(PIN_FET_GREEN, 0);
    fet_set_value(PIN_FET_BLUE, 0);
}

void stage_5(void) {
    // Servo on 10, 11, and 12

    Servo a, b, c;

    a.attach(10);
    b.attach(11);
    c.attach(12);

    #define SERVO_MIN 0
    #define SERVO_MAX 180

    while(current_stage == STAGE_5) {
        logger.info("Servo 1 Min");
        a.write(SERVO_MIN);
        delay(500);

        check_advance();

        logger.info("Servo 1 Max");
        a.write(SERVO_MAX);
        delay(500);

        check_advance();

        logger.info("Servo 2 Min");
        b.write(SERVO_MIN);
        delay(500);

        check_advance();

        logger.info("Servo 2 Max");
        b.write(SERVO_MAX);
        delay(500);

        check_advance();

        logger.info("Servo 3 Min");
        c.write(SERVO_MIN);
        delay(500);

        check_advance();

        logger.info("Servo 3 Max");
        c.write(SERVO_MAX);
        delay(500);

        check_advance();
    }

    a.detach();
    b.detach();
    c.detach();
}

void stage_6(void) {
    logger.info("Stage 6 - Feedback endstop states over serial.");

    uint8_t switches = limit_switches();
    print_switch_status(switches);

    while(current_stage == STAGE_6) {
        if(switches == limit_switches()) {
            check_advance();
        } else {
            switches = limit_switches();
            print_switch_status(switches);
        }
    }
}

void stage_7(void) {
    logger.info("Stage 7 - Nozzle test print. (Use nozzle test hex file)");

    while(current_stage == STAGE_7) {
        check_advance();
    }
}

void (*stage_function[])(void) = {
    &stage_0,
    &stage_1,
    &stage_2,
    &stage_3,
    &stage_4,
    &stage_5,
    &stage_6,
    &stage_7,
};

void run_tests(void) {
    if(current_stage >= STAGE_0 && current_stage <= STAGE_7) {
        stage_function[current_stage]();
    }
}

/*

Stage 1:
Read SD card, report over serial

Stage 2:
Feedback Vadj voltage over serial
Adjust Vadj to 9V

Stage 3:
Blink LED strips in series

Stage 4:
Blink RGB channels in series

Stage 5:
Move servos +SERVO_MAX then SERVO_MIN in series

Stage 6:
Feedback endstop states over serial

Stage 7:
Perform a nozzle test print. It should test one cart, move over, then test the second, then return home

*/
