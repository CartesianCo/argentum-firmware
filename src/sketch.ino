#include "Motor.h"
#include "LEDStrip.h"
#include <SD.h>
#include <Servo.h>
#include "settings.h"
#include "SerialCommand.h"
#include "limit_switch.h"
#include "calibration.h"
#include "cartridge.h"
#include "commands.h"
#include "utils.h"

#include "AccelStepper.h"

Motor aMotor(15, 14, 16, 0); // X
Motor bMotor(18, 17, 19, 0); // Y

Motor *xMotor = &bMotor;
Motor *yMotor = &aMotor;

File myFile;

SerialCommand serial_command;

void setup() {
    Serial.begin(9600);
    Serial.flush();

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

    // Calibration
    serial_command.addCommand("c", &calibrate_command);
    serial_command.addCommand("cs", &calibrate_save_command);

    // Movement
    serial_command.addCommand("m", &move_command);
    serial_command.addCommand("M", &move_command);
    serial_command.addCommand("h", &home_command);

    // Motor
    serial_command.addCommand("x", &power_command);
    serial_command.addCommand("s", &speed_command);

    // Roller Servo
    serial_command.addCommand("l", &lower_command);

    // Print
    serial_command.addCommand("p", &print_command);
    serial_command.addCommand("P", &pause_command);
    serial_command.addCommand("R", &resume_command);

    // Settings
    serial_command.addCommand("?", &read_setting_command);
    serial_command.addCommand("!", &write_setting_command);

    serial_command.addCommand("?L", &read_long_command);
    serial_command.addCommand("!L", &write_long_command);

    serial_command.addCommand("@", &acc);

    serial_command.addCommand("ram", &print_ram);

    serial_command.addCommand("ls", &ls);

    serial_command.addCommand("help", &help_command);

    initLED();

    setLEDToColour(COLOUR_HOME);

    if(!SD.begin(53)) {
        Serial.println("SD card could not be accessed..");
    }

    //uint8_t *firing_buffer = (uint8_t*)malloc(4096);
    help_command();
}

void loop() {
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
