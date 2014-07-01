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

#include "AccelStepper.h"

#define COMMAND_BUFFER_SIZE 64

Motor aMotor(15, 14, 16, 0); // X
Motor bMotor(18, 17, 19, 0); // Y

Motor *xMotor = &bMotor;
Motor *yMotor = &aMotor;

Servo ServoR; // The right drying roller servo
Servo ServoL; // The left drying roller servo

File myFile;

SerialCommand serial_command;

Settings settings;

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

    /*ServoR.attach(14);
    ServoL.attach(15);
    ServoR.write(20);
    ServoL.write(45);*/

    // Calibration
    serial_command.addCommand("c", &calibration);
    serial_command.addCommand("calibrate", &calibration);

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

    initLED();

    AxisSettings asettings;

    /*asettings.axis = 'Z';
    asettings.flipped = false;
    asettings.length = 51234;*/

    //settings.write_axis_settings('X', &asettings);
    settings.read_axis_settings('X', &asettings);

    print_settings(&asettings);

    delay(100);

    setLEDToColour(COLOUR_HOME);

    if(!SD.begin(53)) {
        Serial.println("SD card could not be accessed..");
    }

    //uint8_t *firing_buffer = (uint8_t*)malloc(4096);

    Serial.println("Press p to print Output.hex, S to stop, P to pause, R to resume, c to calibrate.");
}

void print_settings(AxisSettings *settings) {
    Serial.println("Axis Settings:");
    Serial.println(settings->axis);
    Serial.println(settings->flipped);
    Serial.println(settings->length);
}

int free_ram () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void loop() {
    /*Serial.print("RAM: ");
    Serial.println(free_ram());
    delay(500);*/
}

void serialEvent(void) {
    uint8_t input = Serial.read();

    serial_command.add_byte(input);

    if(input == '\r') {
        Serial.println();
    }

    Serial.print((char)input);
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

    uint8_t buffer[4096];

    while(myFile.available()) {
        count++;

        /*if(count % 10000 == 0) {
            Serial.println(count);
        }*/

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
    Serial.println(average);

    // if file.available() fails then do something?

    setLEDToColour(COLOUR_PRINTING);

    // loop through file
    while(myFile.available()) {
        // read in first byte of command
        command[0] = myFile.read();

        // read in extra bytes if necessary
        switch(command[0]) {
            case 1:
                for(int i = 0; i < 7; i++) {
                    command[i + 1] = myFile.read();
                }

                break;

            case 'm':
            case 'M':
                int i = 1;

                while(myFile.peek() != '\n' && i < 9) {
                    command[i] = myFile.read();
                    i++;
                }

                command[i] = '\n';

                break;
        }

        parse_command(command);

        //Check if Any serial commands have been received
        if(Serial.available()) {
            if(Serial.peek() == 'S') {
                myFile.close();

                xMotor->reset_position();
                yMotor->reset_position();

                return;
            }
            serialEvent();
        }
    }

    setLEDToColour(COLOUR_FINISHED);

    //close file
    myFile.close();
}
