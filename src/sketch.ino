#include "Motor.h"
#include "LEDStrip.h"
#include <SD.h>
#include <Servo.h>
#include "settings.h"
#include "SerialCommand.h"
#include "limit_switch.h"

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

    while(1) {
        Serial.println(y_neg_limit());
    }

    /*ServoR.attach(14);
    ServoL.attach(15);
    ServoR.write(20);
    ServoL.write(45);*/

    // Calibration
    serial_command.addCommand("c", &calibration);
    serial_command.addCommand("calibrate", &calibration);

    // Movement
    serial_command.addCommand("m", &move_command);
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
        Serial.println("SD card could not be accessed.");
    }

    Serial.println("Press p to print Output.hex, S to stop, P to pause, R to resume, c to calibrate.");
}

void print_settings(AxisSettings *settings) {
    Serial.println("Axis Settings:");
    Serial.println(settings->axis);
    Serial.println(settings->flipped);
    Serial.println(settings->length);
}

void write_long_command(void) {
    Serial.println("Writing long.");

    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing address parameter");
        return;
    }

    uint8_t address = atoi(arg);

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing value parameter");
        return;
    }

    long value = atol(arg);

    Serial.print(address);
    Serial.print(" = ");
    Serial.println(value);

    settings.write_long(address, value);
}

void read_long_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing address parameter");
        return;
    }

    int address = atoi(arg);

    Serial.print(address);
    Serial.print(" = ");

    long val = settings.read_long((uint8_t)address);
    Serial.println(val);
}

void read_setting_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing address parameter");
        return;
    }

    int address = atoi(arg);

    Serial.print(address);
    Serial.print(" = 0x");

    uint8_t val = settings.read_byte((uint8_t)address);
    Serial.println(val, HEX);
}

void write_setting_command(void) {
    Serial.println("Writing setting.");

    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing address parameter");
        return;
    }

    uint8_t address = atoi(arg);

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing value parameter");
        return;
    }

    uint8_t value = atoi(arg);

    Serial.print(address);
    Serial.print(" = 0x");
    Serial.println(value, HEX);

    settings.write_byte(address, value);
}

void speed_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing speed parameter");
        return;
    }

    long speed = atol(arg);

    if(speed <= 0) {
        speed = 1;
    }

    Motor *motor = motor_from_axis(axis);
    motor->set_speed(speed);
}

void home_command(void) {
    while(!neg_limit()) {
        xMotor->move(-1);
        yMotor->move(-1);
    }

    while(!x_neg_limit()) {
        xMotor->move(-1);
    }

    while(!y_neg_limit()) {
        yMotor->move(-1);
    }
}

void move_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing steps parameter");
        return;
    }

    long steps = atol(arg);

    move(axis, steps);
}

Motor * motor_from_axis(unsigned const char axis) {
    if (toupper(axis) == 'X') {
        return xMotor;
    } else if (toupper(axis) == 'Y') {
        return yMotor;
    }
}

void move(unsigned const char axis, long steps) {
    Motor *motor = motor_from_axis(axis);

    if(steps == 0) {
        motor->reset_position();
    } else {
        motor->move(steps);
    }
}

void power_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(!arg) {
        Serial.println("Missing power parameter");
        return;
    }

    char power = arg[0];

    Motor *motor = NULL;

    if(toupper(axis) == 'X') {
        motor = xMotor;
    } else if(toupper(axis) == 'Y') {
        motor = yMotor;
    } else {
        Serial.println("No axis");
        return;
    }

    if(power == '0') {
        motor->power(0);
    } else if (power == '1') {
        motor->power(1);
    } else {
        Serial.println("Unknown power");
        return;
    }
}

void lower_command(void) {
    Serial.println("Lower/Raise");

    // If it is already raised, lower it
    if (ServoR.read() == 45) {
        ServoR.write(20); //Raised
        ServoL.write(45); //Left servo is opposite

        Serial.println("Raise");

        return;
    }

    // If it is lowered, raise it
    if (ServoR.read() == 20) {
        ServoR.write(45); //Raised
        ServoL.write(20); //Left servo is opposite

        Serial.println("Lower");

        return;
    }
}

void pause_command(void) {
    Serial.println("Paused - enter R to resume");
    while(Serial.read() != 'R');

    Serial.println("Resuming");
}

void resume_command(void) {
    Serial.println("Resuming");
}

void print_command(void) {
    Serial.println("Printing file.");

    for (int i=0; i <= 0; i++) {
        readFile("Output.hex");
    }
}

void loop() {
}

void serialEvent(void) {
    uint8_t input = Serial.read();

    serial_command.add_byte(input);

    if(input == '\r') {
        Serial.println();
    }

    Serial.print((char)input);
}

void sserialEvent(void) {
    byte command[10];
    int i;

    // Loop through all serial data
    while (Serial.available()) {

        // read in first byte of command
        command[0] = Serial.read();

        // read in extra bytes if necessary
        switch(command[0]) {
            case 'p':
                break;

            case 'm':
            case 'M':
                i = 1;
                while(Serial.peek() == -1) {}

                while(Serial.peek() != 10 && i < 9) {
                    command[i] = Serial.read();
                    i++;

                    while(Serial.peek() == -1){}
                }
                command[i] = '\n';

                break;

            case 'X':
            case 'Y':
            case 'x':
            case 'y':
                while(Serial.peek() == -1){}
                command[1] = Serial.read();

                break;
        }

        //run command
        parse_command(command);
    }
}

void parse_command(byte* command) {
    int dist = 0;

    switch(command[0]) {
        case 0x01:
            fireHead((byte)command[1], (byte)command[2], (byte)command[5], (byte)command[6]);

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

const static int a_escape_steps = 400;
const static int b_escape_steps = 400;

/**
 * Attempt to resolve the direction and a axis of a motor.
 *
 * Assumptions:
 *
 * Only one switch is affected by the movement, and the axes are long enough such
 * that the supplied movement distance won't release and trigger the limits of
 * that axis.
 *
 * The (released | triggered) logic simplification relies on this.
 *
 * @param motor The motor to attempt to resolve
 * @param steps The number of steps to move that motor
 * @param axis_mask A mask defining the axis that this motor should correspond
 to.

 * @param axis_correct A pointer to a bool, which will indicate whether the
 axis_mask corresponds to any of the limit switches that were triggered or
 released.

 * @param direction_mask A mask defining the direction that this motor should
 move in for the given step, typically positive.

 * @param direction_correct A pointer to a bool...
 */
bool resolve(Motor *motor,
             long steps,
             uint8_t axis_mask,
             bool *axis_correct,
             uint8_t direction_mask,
             bool *direction_correct) {

    uint8_t before = limit_switches();
    motor->move(steps);
    uint8_t after = limit_switches();

    uint8_t released = (before & ~after);
    uint8_t triggered = (after & ~before);

    // If we activated a switch, release it. Should this happen?
    if(triggered) {
        motor->move(-steps);
    }

    if(released || triggered) {
        *axis_correct = ((released | triggered) & axis_mask);

        *direction_correct = ((released | triggered) & direction_mask);

        if(released) {
            *direction_correct = !(*direction_correct);
        }

        return true;
    }

    return false;
}

bool freedom(bool *x_direction_resolved, bool *y_direction_resolved) {
    *x_direction_resolved = false;
    *y_direction_resolved = false;

    bool a_resolved = false;
    bool b_resolved = false;

    if(any_limit()) {
        bool axis_correct = false;
        bool direction_correct = false;

        // A -> X+
        a_resolved = resolve(&aMotor, a_escape_steps, X_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!a_resolved) {
            a_resolved = resolve(&aMotor, -a_escape_steps, X_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(a_resolved) {
            if(axis_correct) {
                Serial.println("Found A = X");

                xMotor = &aMotor;
                yMotor = &bMotor;

                *x_direction_resolved = true;
            } else {
                Serial.println("Found A = Y");

                xMotor = &bMotor;
                yMotor = &aMotor;

                *y_direction_resolved = true;
            }

            if(direction_correct) {
                Serial.println("Found + = +");
            } else {
                Serial.println("Found + = -");
                aMotor.set_inverted(true);
            }
        }

        // B -> Y+
        b_resolved = resolve(&bMotor, b_escape_steps, Y_MASK, &axis_correct, POS_MASK, &direction_correct);

        if(!b_resolved) {
            b_resolved = resolve(&bMotor, -b_escape_steps, Y_MASK, &axis_correct, NEG_MASK, &direction_correct);
        }

        if(b_resolved) {
            if(axis_correct) {
                Serial.println("Found B = Y");
                xMotor = &aMotor;
                yMotor = &bMotor;

                *y_direction_resolved = true;
            } else {
                Serial.println("Found B = X");
                xMotor = &bMotor;
                yMotor = &aMotor;

                *x_direction_resolved = true;
            }

            if(direction_correct) {
                Serial.println("Found + = +");
            } else {
                Serial.println("Found + = -");
                bMotor.set_inverted(true);
            }
        }
    } else {
        Serial.println("No switches are initially triggered.");
    }

    return (a_resolved | b_resolved);
}

void calibration(void) {
    Serial.println("Calibration beginning.");

    xMotor->set_direction(Motor::Forward);
    yMotor->set_direction(Motor::Forward);

    xMotor->set_speed(250);
    yMotor->set_speed(250);

    long x_distance = 0L;
    long y_distance = 0L;

    bool axes_resolved = false;
    bool x_direction_resolved = false;
    bool y_direction_resolved = false;

    axes_resolved = freedom(&x_direction_resolved, &y_direction_resolved);

    xMotor->set_speed(4000);
    yMotor->set_speed(4000);

    if(!axes_resolved) {
        Serial.println("Resolved nothing");
        Serial.println("Finding X");

        while(!any_limit()) {
            xMotor->move(-1);
        }

        if(y_limit()) {
            // Incorrect
            Motor *temp = xMotor;

            xMotor = yMotor;
            yMotor = temp;
        }
    }

    while(!(x_direction_resolved && y_direction_resolved)) {
        if(!x_direction_resolved) {
            if(!x_limit()) {
                xMotor->move(-1);
            } else {
                x_direction_resolved = true;

                if(x_pos_limit()) {
                    // Inverted
                    xMotor->set_inverted(true);
                }
            }
        }

        if(!y_direction_resolved) {
            if(!y_limit()) {
                yMotor->move(-1);
            } else {
                y_direction_resolved = true;

                if(y_pos_limit()) {
                    // Inverted
                    yMotor->set_inverted(true);
                }
            }
        }
    }

    Serial.println("Homing");

    while(!pos_limit()) {
        xMotor->move(1);
        yMotor->move(1);
    }

    while(!x_pos_limit()) {
        xMotor->move(1);
    }

    while(!y_pos_limit()) {
        yMotor->move(1);
    }

    while(!neg_limit()) {
        xMotor->move(-1);
        yMotor->move(-1);

        x_distance++;
        y_distance++;
    }

    while(!x_neg_limit()) {
        xMotor->move(-1);
        x_distance++;
    }

    while(!y_neg_limit()) {
        yMotor->move(-1);
        y_distance++;
    }

    Serial.println("Calibration procedure completed:");
    Serial.println(x_distance, DEC);
    Serial.println(y_distance, DEC);
}

// PORT C is [R1, R2, R3, R4, L1, L2, L3, L4] (Multiplexer)
// PORT L is Left (MOSFET Drivers)
// PORT A is Right (MOSFET Drivers)

// Put in to a printhead class
void fireHead(byte rPrim, byte rAddr, byte lPrim, byte lAddr) {
    int x1, x2;

    if (rPrim || rAddr || lPrim || lAddr) {
        PORTC = (PORTC | int(lAddr)); //Assign this to port C to load it into the cannon.

        //x1 = int(rAddr) << 4 | int(rAddr) >4; //Reverse
        //x2 = (x1 & 0x33) << 2 | (x1 & 0xcc) >2;
        //PORTC = (PORTC | ((x2 & 0x55) << 1 | (x2 & 0xaa) >1);

        PORTC = (PORTC | (int(rAddr) << 4));

        __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

        PORTL = int(lPrim);
        PORTA = int(rPrim);

        delayMicroseconds(6);

        __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

        PORTA = 0;
        PORTL = 0;

        __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

        PORTC = 0;
        //delayMicroseconds(500);
    }
}
