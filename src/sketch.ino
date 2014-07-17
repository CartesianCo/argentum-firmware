#include "Motor.h"
#include "LEDStrip.h"
#include <SD.h>
#include <Servo.h>
#include "settings.h"
#include "SerialCommand.h"
//#include "limit_switch.h"
#include "calibration.h"
#include "cartridge.h"
#include "commands.h"
#include "utils.h"
#include "axis.h"

#include "comms.h"

#include "logging.h"

#include "rollers.h"

#include "argentum.h"

#include "AccelStepper.h"

// step_pin, dir_pin, power_pin, steps_per_rev
//Motor aMotor(15, 14, 16, 0); // X
//Motor bMotor(18, 17, 19, 0); // Y

//Motor *xMotor = &aMotor;
//Motor *yMotor = &bMotor;

File myFile;

class Printer {
public:
    enum States {
        Idle,
        Calibrating,
        Moving,
        Printing,
    };
};

uint8_t current_state = Printer::Idle;

void setup() {
    comms.initialise();

    logger.minimum_log_level = Logger::Info;
    logger.enabled = true;

    logger.info() << "Information" << Comms::endl;
    logger.warn() << "Warning" << Comms::endl;
    logger.error() << "Error" << Comms::endl;

    rollers.disable();
    rollers.enable();

    //xMotor->set_speed(1500);
    //yMotor->set_speed(1500);
    x_axis.set_speed(1500);
    y_axis.set_speed(1500);

    settings_initialise(false);

    // Configure Cartridge Ports
    DDRC = 0xFF;
    DDRL = 0xFF;
    DDRA = 0xFF;

    // Configure Input Pins
    pinMode(A12, INPUT); // General Analog Inputs
    pinMode(A13, INPUT);
    pinMode(A14, INPUT);
    pinMode(A15, INPUT); // Voltage Feedback (9V Sense)

    pinMode(5, INPUT); // XMAX
    pinMode(A0, INPUT); // XMIN

    pinMode(A1, INPUT); // YMAX
    pinMode(6, INPUT); // YMIN

    // Disable FETs by default
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    //analogWrite(7, 128);

    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
    //analogWrite(8, 128);

    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
    //analogWrite(9, 128);

    // Calibration
    serial_command.addCommand("c", &calibrate_command);
    //serial_command.addCommand("cl", &calibrate_loop_command);

    // Movement
    serial_command.addCommand("m", &move_command);
    serial_command.addCommand("M", &move_command);
    serial_command.addCommand("h", &home_command);

    serial_command.addCommand("0", &goto_zero_command);
    serial_command.addCommand(")", &zero_position_command);
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
    //serial_command.addCommand("stat", &stat_command);

    serial_command.addCommand("size", &size_command);

    serial_command.addCommand("++", &plus_command);
    serial_command.addCommand("--", &minus_command);
    serial_command.addCommand("wait", &wait_command);


    // Common
    serial_command.addCommand("help", &help_command);

    initLED();

    setLEDToColour(COLOUR_HOME);

    init_sd_command();

    // Initialise Axes from EEPROM here

    //uint8_t *firing_buffer = (uint8_t*)malloc(4096);
    help_command();
}

static int white = 0;
static long old_time = 0;
static bool dir = false;


// Note: This loop _should_ execute three times faster than the motors can step
// at 5000 speed. Measured.
void loop() {
    x_axis.run();
    y_axis.run();

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

    switch(current_state) {
        case Printer::Idle:
        break;

        case Printer::Calibrating:
        break;

        case Printer::Moving:
        break;

        case Printer::Printing:
        break;

        default:
            current_state = Printer::Idle;
        break;
    }
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

void swap_motors(void) {
    /*Motor *temp = xMotor;
    xMotor = yMotor;
    yMotor = temp;*/
    ProtoMotor *temp = x_axis.motor;

    x_axis.motor = y_axis.motor;
    y_axis.motor = temp;
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

void file_stats(char *filename) {
    uint8_t command[10];

    myFile = SD.open(filename);

    // Check if file open succeeded, if not output error message
    if (!myFile) {
        Serial.print("File could not be opened: ");
        Serial.println(filename);

        return;
    }

    Serial.println("Starting");

    long max_x = 0;
    long max_y = 0;

    long cur_x = 0;
    long cur_y = 0;

    // loop through file
    while(myFile.available()) {
        // read in first byte of command
        command[0] = myFile.read();

        //Serial.println(command[0]);

        if(command[0] == 'M') {
            // read in extra bytes if necessary
            int i = 1;

            while(myFile.peek() != '\n' && i < 8) {
                command[i] = myFile.read();
                i++;
            }
            command[i] = 0x00;

            long steps = atol((const char *)&command[4]);
            char axis = command[2];

            //logger.info() << "Movement command: " << axis << " " << steps << Comms::endl;

            if(axis == 'X') {
                cur_x += steps;

                if(cur_x > max_x) {
                    max_x = cur_x;
                }
            }

            if(axis == 'Y') {
                cur_y += abs(steps);

                if(cur_y > max_y) {
                    max_y = cur_y;
                }

                logger.info() << "steps: " << steps << " cur_y: " << cur_y
                        << " max_y: " << max_y << Comms::endl;
            }
        }
    }

    logger.info() << "File dimensions: " << max_x << " x " << max_y << " steps"
            << Comms::endl;

    //close file
    myFile.close();
}

bool readFile(char *filename) {
    byte command[10];

    swap_motors();

    //xMotor->set_position(0L);
    //yMotor->set_position(0L);

    x_axis.zero();
    y_axis.zero();

    // Open File
    myFile = SD.open(filename);

    // Check if file open succeeded, if not output error message
    if (!myFile) {
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

    setLEDToColour(COLOUR_PRINTING);

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
        } else if(command[0] == 'M') {
            // read in extra bytes if necessary
            int i = 1;

            while(myFile.peek() != '\n' && i < 8) {
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
            }

            if(axis == 'Y') {
                cur_y += abs(steps);

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

                swap_motors();

                Serial.println("Stopping.");

                home_command();

                return false;
            }
            serialEvent();
        }
    }

    setLEDToColour(COLOUR_FINISHED);

    logger.info() << "File dimensions: " << max_x << " x " << max_y << " steps"
            << Comms::endl;

    x_size = max_x;
    y_size = max_y;

    //close file
    myFile.close();

    swap_motors();

    return true;
}
