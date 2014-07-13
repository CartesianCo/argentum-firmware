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

#include "logging.h"

#include "rollers.h"
#include "limit.h"

#include "AccelStepper.h"

// step_pin, dir_pin, power_pin, steps_per_rev
Motor aMotor(15, 14, 16, 0); // X
Motor bMotor(18, 17, 19, 0); // Y

ProtoMotor xProto(15, 14, 16);
ProtoMotor yProto(18, 17, 19);

Motor *xMotor = &aMotor;
Motor *yMotor = &bMotor;

Rollers rollers;

File myFile;

SerialCommand serial_command;

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

bool dummy_limit(void) {
    return false;
}

Axis x_axis(Axis::X, &xProto, &Limit::x_positive, &Limit::x_negative);
Axis y_axis(Axis::Y, &yProto, &Limit::y_positive, &Limit::y_negative);

void setup() {
    Serial.begin(115200);
    Serial.flush();

    logger.minimum_log_level = Logger::Info;

    logger.info() << "Information" << Logger::endl;
    logger.warn() << "Warning" << Logger::endl;
    logger.error() << "Error" << Logger::endl;

    //pinMode(12, OUTPUT);

    rollers.disable();
    rollers.enable();

    xMotor->set_speed(1500);
    yMotor->set_speed(1500);

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
    serial_command.addCommand("cl", &calibrate_loop_command);

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
    serial_command.addCommand("=", &motors_on_command);
    serial_command.addCommand("-", &motors_off_command);
    serial_command.addCommand("_", &motors_off_command);

    // Roller Servo
    serial_command.addCommand("l", &rollers_command);

    // Print
    serial_command.addCommand("p", &print_command);
    serial_command.addCommand("P", &pause_command);
    serial_command.addCommand("R", &resume_command);

    // Settings
    serial_command.addCommand("?", &read_setting_command);
    serial_command.addCommand("??", &read_saved_setting_command);
    serial_command.addCommand("!", &write_setting_command);

    // Experimentals
    serial_command.addCommand("@", &acc);
    serial_command.addCommand("lim", &limit_switch_command);
    serial_command.addCommand("ram", &print_ram);

    serial_command.addCommand("digital", &digital_command);
    serial_command.addCommand("analog", &analog_command);

    // SD Card
    serial_command.addCommand("ls", &ls);
    serial_command.addCommand("sd", &init_sd_command);

    // Colour
    serial_command.addCommand("red", &red_command);
    serial_command.addCommand("green", &green_command);
    serial_command.addCommand("blue", &blue_command);

    serial_command.addCommand("pwm", &pwm_command);

    serial_command.addCommand("sweep", &sweep_command);

    serial_command.addCommand("abs", &proto_move);
    serial_command.addCommand("xpos", &axis_pos);


    // Common
    serial_command.addCommand("help", &help_command);

    initLED();

    setLEDToColour(COLOUR_HOME);

    init_sd_command();

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
    Motor *temp = xMotor;
    xMotor = yMotor;
    yMotor = temp;
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

void readFile(char* filename) {
    byte command[10];

    swap_motors();

    // Open File
    myFile = SD.open(filename);

    // Check if file open succeeded, if not output error message
    if (!myFile) {
        Serial.print("File could not be opened: ");
        Serial.println(filename);

        return;
    }

    Serial.println("Starting");
    //Serial.println(start);

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

                return;
            }
            serialEvent();
        }
    }

    setLEDToColour(COLOUR_FINISHED);

    //close file
    myFile.close();

    swap_motors();
}
