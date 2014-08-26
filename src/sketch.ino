#include "util/colour.h"
//#include <SD.h>
#include "util/settings.h"
#include "util/SerialCommand.h"
#include "argentum/calibration.h"
#include "util/cartridge.h"
#include "argentum/commands.h"
#include "util/utils.h"
#include "util/axis.h"
#include "util/logging.h"
#include "argentum/argentum.h"

#include "argentum/boardtests.h"

#include "util/SdFat/SdFat.h"

#define FIRMWARE_VERSION "v0.12"

SdFile myFile;

void setup() {
    comms.initialise();

    logger.minimum_log_level = Logger::Info;
    logger.enabled = true;

    logger.info("Argentum " FIRMWARE_VERSION);

    colour_init();
    colour(COLOUR_HOME);

    init_sd_command();

    rollers.disable();
    rollers.enable();
    rollers.retract();

    x_axis.set_speed(1500);
    y_axis.set_speed(1500);

    settings_initialise(true);

    cartridge_initialise();
    analog_initialise();
    limit_initialise();
    fet_initialise();

    // Calibration
    serial_command.addCommand("c", &calibrate_command);

    // Movement
    serial_command.addCommand("m", &move_command);
    serial_command.addCommand("M", &move_command);

    serial_command.addCommand("0", &goto_zero_command);
    serial_command.addCommand(")", &zero_position_command);
    serial_command.addCommand("pos", &current_position_command);

    serial_command.addCommand("h", &home_command);
    serial_command.addCommand("b", &back_corner_command);

    serial_command.addCommand("stest", &stepper_test_command);

    // Motor
    serial_command.addCommand("s", &speed_command);

    serial_command.addCommand("+", &motors_on_command);
    serial_command.addCommand("-", &motors_off_command);

    // Roller Servo
    serial_command.addCommand("l", &rollers_command);

    // Print
    serial_command.addCommand("p", &print_command);
    serial_command.addCommand("P", &pause_command);
    serial_command.addCommand("R", &resume_command);

    // Settings
    serial_command.addCommand("?", &read_setting_command);
    serial_command.addCommand("?eeprom", &read_saved_setting_command);
    serial_command.addCommand("!write", &write_setting_command);
    serial_command.addCommand("defaults", &default_settings_command);

    // Experimentals
    serial_command.addCommand("lim", &limit_switch_command);
    serial_command.addCommand("ram", &print_ram);

    //serial_command.addCommand("digital", &digital_command);
    //serial_command.addCommand("analog", &analog_command);

    // SD Card
    serial_command.addCommand("ls", &ls);
    serial_command.addCommand("sd", &init_sd_command);

    // Colour
    serial_command.addCommand("red", &red_command);
    serial_command.addCommand("green", &green_command);
    serial_command.addCommand("blue", &blue_command);

    serial_command.addCommand("pwm", &pwm_command);

    serial_command.addCommand("sweep", &sweep_command);

    serial_command.addCommand("abs", &absolute_move);
    serial_command.addCommand("inc", &incremental_move);

    //serial_command.addCommand("xpos", &axis_pos);

    serial_command.addCommand("++", &plus_command);
    serial_command.addCommand("--", &minus_command);
    serial_command.addCommand("wait", &wait_command);

    serial_command.addCommand("volt", &primitive_voltage_command);

    // Common
    serial_command.addCommand("help", &help_command);

    load_settings();

    limit_switch_command();

    //uint8_t *firing_buffer = (uint8_t*)malloc(4096);
    //help_command();
    logger.info("Ready");
}

static int white = 0;
static long old_time = 0;
static bool dir = false;

// Note: This loop _should_ execute three times faster than the motors can step
// at 5000 speed. Measured.
void loop() {
    x_axis.run();
    y_axis.run();

    //run_tests();

    /*if(millis() - old_time > 10) {
        if(!dir) {
            white++;
        } else {
            white--;
        }

        if(white >= 255) {
            dir = true;
        }

        if(white <= 0) {
            dir = false;
        }

        old_time = millis();
        analogWrite(8, white);
    }*/
}

void serialEvent(void) {
    uint8_t input = Serial.read();

    if(input == 0x08) {
        Serial.print("\x08 ");
    }

    if(input == '\r') {
        Serial.print("\r\n");
    } else {
        Serial.print((char)input);
    }

    serial_command.add_byte(input);
}

void parse_command(byte* command) {
    int dist = 0;

    switch(command[0]) {
        case 0x01:
            fire_head((byte)command[1], (byte)command[2], (byte)command[5], (byte)command[6]);

            break;

        default:
            break;
    }
}

bool readFile(char *filename) {
    byte command[10];

    // Open File
    myFile.open(filename);

    // Check if file open succeeded, if not output error message
    if (!myFile.isOpen()) {
        logger.error() << "File could not be opened: " << filename
                << Comms::endl;

        return false;
    }

    logger.info() << "readFile(" << filename << ")" << Comms::endl;

    long start = micros();
    long end = 0L;
    long count = 0L;

    colour(COLOUR_PRINTING);

    // loop through file
    while(myFile.available()) {
        // read in first byte of command
        command[0] = myFile.read();

        if(command[0] == 0x01) {
            // read in extra bytes if necessary
            switch(command[0]) {
                case 1:
                    for(int i = 0; i < 7; i++) {
                        command[i + 1] = myFile.read();
                    }

                    break;
            }

            parse_command(command);
        } else {
            serial_command.add_byte(command[0]);
        }

        if(Serial.available()) {
            if(Serial.peek() == 'S') {
                myFile.close();

                Serial.println("Stopping.");

                goto_zero_command();

                return false;
            }
            serialEvent();
        }
    }

    colour(COLOUR_FINISHED);

    myFile.close();

    return true;
}
