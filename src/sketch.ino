#include "util/colour.h"
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

SdFile myFile;

void setup() {
    comms.initialise();

    logger.minimum_log_level = Logger::Info;
    logger.enabled = true;

    colour_init();
    colour(COLOUR_HOME);

    rollers.disable();

    x_axis.set_speed(1500);
    y_axis.set_speed(1500);

    settings_initialise();

    cartridge_initialise();
    analog_initialise();
    limit_initialise();
    fet_initialise();

    // Calibration
    serial_command.addCommand("c", &calibrate_command);
    //serial_command.addCommand("cl", &calibrate_loop_command);

    // Movement
    serial_command.addCommand("m", &move_command);
    serial_command.addCommand("M", &move_command);

    serial_command.addCommand("0", &goto_zero_command);
    serial_command.addCommand(")", &zero_position_command);
    serial_command.addCommand("home", &home_command);
    serial_command.addCommand("pos", &current_position_command);


    // Motor
    serial_command.addCommand("x", &power_command);
    serial_command.addCommand("s", &speed_command);

    serial_command.addCommand("+", &motors_on_command);
    //serial_command.addCommand("=", &motors_on_command);
    serial_command.addCommand("-", &motors_off_command);
    //serial_command.addCommand("_", &motors_off_command);

    // Roller Servo
    serial_command.addCommand("l", &rollers_command);

    // Print
    serial_command.addCommand("p", &print_command);
    serial_command.addCommand("P", &pause_command);
    serial_command.addCommand("R", &resume_command);
    serial_command.addCommand("F", &fire_command);
    serial_command.addCommand("D", &draw_command);

    // Settings
    serial_command.addCommand("?", &read_setting_command);
    serial_command.addCommand("?eeprom", &read_saved_setting_command);
    serial_command.addCommand("!write", &write_setting_command);

    // Experimentals
    //serial_command.addCommand("@", &acc);
    serial_command.addCommand("lim", &limit_switch_command);
    serial_command.addCommand("ram", &print_ram);

    //serial_command.addCommand("digital", &digital_command);
    //serial_command.addCommand("analog", &analog_command);

    // SD Card
    serial_command.addCommand("ls", &ls_command);
    serial_command.addCommand("rm", &rm_command);
    serial_command.addCommand("md5", &md5_command);
    serial_command.addCommand("djb2", &djb2_command);
    serial_command.addCommand("sd", &init_sd_command);
    serial_command.addCommand("recv", &recv_command);

    // Colour
    serial_command.addCommand("red", &red_command);
    serial_command.addCommand("green", &green_command);
    serial_command.addCommand("blue", &blue_command);

    serial_command.addCommand("pwm", &pwm_command);

    serial_command.addCommand("sweep", &sweep_command);

    serial_command.addCommand("abs", &absolute_move);
    serial_command.addCommand("inc", &incremental_move);

    serial_command.addCommand("++", &plus_command);
    serial_command.addCommand("--", &minus_command);
    serial_command.addCommand("wait", &wait_command);

    serial_command.addCommand("volt", &primitive_voltage_command);

    // Common
    serial_command.addCommand("help", &help_command);
    serial_command.addCommand("version", &version_command);
    serial_command.addCommand("pnum", &printer_number_command);
    serial_command.addCommand("sle", &sle_command);

    // Initialise Axes from EEPROM here
    if(global_settings.calibration.x_axis.motor == 'A') {
        x_axis.set_motor(&a_motor);
    } else {
        x_axis.set_motor(&b_motor);
    }

    if(global_settings.calibration.y_axis.motor == 'A') {
        y_axis.set_motor(&a_motor);
    } else {
        y_axis.set_motor(&b_motor);
    }

    if(global_settings.calibration.x_axis.flipped) {
        x_axis.set_motor_mapping(Axis::CW_Negative);
    } else {
        x_axis.set_motor_mapping(Axis::CW_Positive);
    }

    if(global_settings.calibration.y_axis.flipped) {
        y_axis.set_motor_mapping(Axis::CW_Negative);
    } else {
        y_axis.set_motor_mapping(Axis::CW_Positive);
    }

    x_axis.length = global_settings.calibration.x_axis.length;
    y_axis.length = global_settings.calibration.y_axis.length;

    //uint8_t *firing_buffer = (uint8_t*)malloc(4096);
    printer_number_command();
    version_command();
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

bool simulateLocalEcho = false;
void sle_command(void) {
    simulateLocalEcho = !simulateLocalEcho;
}

void serialEvent(void) {
    uint8_t input = Serial.read();

    if (simulateLocalEcho)
    {
        if(input == 0x08) {
            Serial.print("\x08 ");
        }

        if(input == '\r') {
            Serial.print("\r\n");
        } else {
            Serial.print((char)input);
        }
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

extern void fire_spec(char *spec);

bool readFile(char *filename) {
    byte command[20];

    //swap_motors();

    //xMotor->set_position(0L);
    //yMotor->set_position(0L);

    //x_axis.zero();
    //y_axis.zero();

    // Open File
    myFile.open(filename);

    // Check if file open succeeded, if not output error message
    if (!myFile.isOpen()) {
        Serial.print("File could not be opened: ");
        Serial.println(filename);

        return false;
    }

    //Serial.println("Starting");
    //Serial.println(start);
    logger.info() << "readFile(" << filename << ")" << Comms::endl;

    long start = micros();
    long end = 0L;
    long count = 0L;

    /*uint8_t buffer[4096];

    while(myFile.available()) {
        count++;

        myFile.read(buffer, sizeof(buffer));
    }

    end = micros();

    count = count * sizeof(buffer);

    double time = end - start;
    double average = time / count;

    Serial.println("Timing:");
    Serial.println(start);
    Serial.println(end);
    Serial.println(time);
    Serial.println(count);
    Serial.println(average);*/

    // if file.available() fails then do something?

    colour(COLOUR_PRINTING);

    long max_x = 0;
    long max_y = 0;

    long cur_x = 0;
    long cur_y = 0;

    // loop through file
    while(myFile.available()) {
        // read in first byte of command
        command[0] = myFile.read();

        //Serial.println(command[0]);

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
        } else if(command[0] == 'F') {
            // Textual version of firing commands
            int i = 1;

            while(myFile.peek() != '\n' && i < 18) {
                command[i] = myFile.read();
                i++;
            }
            command[i] = 0x00;

            fire_spec((char*)command + 2);
        } else if(command[0] == 'M') {
            // read in extra bytes if necessary
            int i = 1;

            while(myFile.peek() != '\n' && i < 18) {
                command[i] = myFile.read();
                i++;
            }
            command[i] = 0x00;

            long steps = atol((const char *)&command[4]);
            char axis = command[2];

            //logger.info() << "Movement command: " << axis << " " << steps << Comms::endl;

            if(axis == 'X') {
                cur_x += steps;

                if(steps == 0) {
                    cur_x = 0;
                }

                if(cur_x > max_x) {
                    max_x = cur_x;
                }

                Serial.println(".");
            }

            if(axis == 'Y') {
                cur_y += steps;

                if(steps == 0) {
                    cur_y = 0;
                }

                if(cur_y > max_y) {
                    max_y = cur_y;
                }

                //logger.info() << "steps: " << steps << " cur_y: " << cur_y
                //        << " max_y: " << max_y << Comms::endl;
            }

            for(int i = 0; i < 10; i++) {
                serial_command.add_byte(command[i]);

                if(command[i] == '\n') {
                    break;
                }
            }
        } else {
            serial_command.add_byte(command[0]);
        }

        //Check if Any serial commands have been received
        if(Serial.available()) {
            if(Serial.peek() == 'S') {
                myFile.close();

                //swap_motors();

                Serial.println("Stopping.");

                goto_zero_command();

                return false;
            }
            serialEvent();
        }
    }

    colour(COLOUR_FINISHED);

    logger.info() << "File dimensions: " << max_x << " x " << max_y << " steps"
            << Comms::endl;

    x_size = max_x;
    y_size = max_y;

    //close file
    myFile.close();

    //swap_motors();

    return true;
}
