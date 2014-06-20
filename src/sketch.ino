#include "Motor.h"
#include "LEDStrip.h"
#include <SD.h>
#include <Servo.h>
#include "settings.h"

Motor aMotor(15, 14, 16, 0); // X
Motor bMotor(18, 17, 19, 0); // Y

Motor *xMotor = &bMotor;
Motor *yMotor = &aMotor;

Servo ServoR; // The right drying roller servo
Servo ServoL; // The left drying roller servo

File myFile;

void setup() {
    /*while(1) {
        xMotor->set_direction(Motor::Forward);

        for(int i = 0; i < 200; i++) {
            xMotor->step();
        }

        xMotor->set_direction(Motor::Backward);

        for(int i = 0; i < 200; i++) {
            xMotor->step();
        }
    }*/

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

    initLED();

    delay(1000);

    setLEDToColour(COLOUR_HOME);

    if(!SD.begin(53)) { //Initalise SD
        Serial.println("SD card could not be accessed.");
    }

    Serial.println("Press p to print Output.txt, S to stop, P to pause, R to resume");
    //xMotor.setSpeed(5000);
}

void loop() {
}


void serialEvent() {
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
        parseCommand(command);
    }
}


void parseCommand(byte* command) {
    int dist = 0;

    switch(command[0]) {
        case 0x01:
            fireHead((byte)command[1], (byte)command[2], (byte)command[5], (byte)command[6]);

            break;

        case 'p':
            Serial.println("Printing file.");

            for (int i=0; i <= 0; i++) {
                readFile("Output.hex");
            }

            break;

        case 'P': // P
            Serial.println("Paused - enter R to resume");
            while(Serial.read() != 'R');

            Serial.println("Resuming");

            break;

        case 'm':
        case 'M':
            char cmdInt[5];

            for(int i = 0; i < 5; i++) {
                cmdInt[i] = command[4+i];
            }

            dist = atoi(cmdInt);

            Serial.print("Moving ");
            Serial.println(dist);

            if (toupper(command[2]) == 'X') {
                if(dist == 0) {
                    xMotor->reset_position();
                } else {
                    xMotor->move(dist);
                }
            } else if (toupper(command[2]) == 'Y') {
                if(dist == 0) {
                    yMotor->reset_position();
                } else {
                    yMotor->move(dist);
                }
            }

            break;

        case 'C':
            calibration();

            break;

        case '?':
            Serial.print(command[1]);
            Serial.print(" = ");
            Serial.println(read_setting(command[1]));

            break;

        case '!':
            Serial.println("Writing setting.");

            write_setting(command[1], command[2]);

            break;

        case 'x':
        case 'X':
            if(command[1] == '0') {
                xMotor->power(0);
            } else if (command[1] == '1') {
                xMotor->power(1);
            } else {
                Serial.println(command[1]);
            }

            break;

        case 'y':
        case 'Y':
            if(command[1] == '0') {
                yMotor->power(0);
            } else if (command[1] == '1') {
                yMotor->power(1);
            } else {
                Serial.println(command[1]);
            }

            break;

        case 'l':
            Serial.println("Lower/Raise");

            // If it is already raised, lower it
            if (ServoR.read() == 45) {
                ServoR.write(20); //Raised
                ServoL.write(45); //Left servo is opposite

                Serial.println("Raise");

                break;
            }

            // If it is lowered, raise it
            if (ServoR.read() == 20) {
                ServoR.write(45); //Raised
                ServoL.write(20); //Left servo is opposite

                Serial.println("Lower");
                break;
            }

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

        parseCommand(command);

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

const int x_pos_limit_pin = 5;
const int x_neg_limit_pin = A0;
const int y_pos_limit_pin = A1;
const int y_neg_limit_pin = 6;

/*
#define x_pos_limit() digitalRead(x_pos_limit_pin)
#define x_neg_limit() digitalRead(x_neg_limit_pin)

#define y_pos_limit() digitalRead(y_pos_limit_pin)
#define y_neg_limit() digitalRead(y_neg_limit_pin)

#define x_limit() (x_pos_limit() || x_neg_limit())
#define y_limit() (y_pos_limit() || y_neg_limit())

#define any_limit() (x_limit() || y_limit())
*/

#define X_POS_BIT 0b00001000
#define X_NEG_BIT 0b00000100
#define Y_POS_BIT 0b00000010
#define Y_NEG_BIT 0b00000001

#define X_POS(switches) (switches & X_POS_BIT)
#define X_NEG(switches) (switches & X_NEG_BIT)
#define Y_POS(switches) (switches & Y_POS_BIT)
#define Y_NEG(switches) (switches & Y_NEG_BIT)

uint8_t limit_switches(void) {
    uint8_t switches = 0b00000000;

    if(x_pos_limit()) {
        switches |= 0b00001000;
    }

    if(x_neg_limit()) {
        switches |= 0b00000100;
    }

    if(y_pos_limit()) {
        switches |= 0b00000010;
    }

    if(y_neg_limit()) {
        switches |= 0b00000001;
    }

    return switches;
}

bool x_pos_limit(void) {
    return digitalRead(x_pos_limit_pin);
}

bool x_neg_limit(void) {
    return digitalRead(x_neg_limit_pin);
}

bool y_pos_limit(void) {
    return digitalRead(y_pos_limit_pin);
}

bool y_neg_limit(void) {
    return digitalRead(y_neg_limit_pin);
}

bool x_limit(void) {
    return (x_pos_limit() || x_neg_limit());
}

bool y_limit(void) {
    return (y_pos_limit() || y_neg_limit());
}

bool any_limit(void) {
    return (x_limit() || y_limit());
}

void swap_motors(void) {
    xMotor = &bMotor;
    yMotor = &aMotor;
}

void print_switch_status(uint8_t switches) {
    //Serial.print("Switch binary: ");
    //Serial.println(switches, BIN);

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

uint8_t switch_change(void) {
    static uint8_t old_switches = limit_switches();

    uint8_t current_switches = limit_switches();

    //Serial.println(old_switches, BIN);
    //Serial.println(current_switches, BIN);

    uint8_t diff = old_switches ^ current_switches;

    old_switches = current_switches;

    return diff;
}

const static int x_escape_steps = 200;
const static int y_escape_steps = 400;

bool clear_unknown_blockage(void) {
    if(any_limit()) {
        Serial.println("At least one switch is already triggered.");
        print_switch_status();

        uint8_t switches = switch_change();

        Serial.println("Moving X + to escape");
        xMotor->move(x_escape_steps);

        Serial.println("Result of moving X + :");
        switches = switch_change();
        print_switch_status(switches);

        if(any_limit()) {
            Serial.println("Still triggered...");
            //print_switch_status();

            Serial.println("Moving X - to escape");

            xMotor->move(-x_escape_steps);
        } else {
            Serial.println("Moving X + worked");
            //print_switch_status();

            return true;
        }

        Serial.println("Result of moving X - :");
        switches = switch_change();
        print_switch_status(switches);

        if(any_limit()) {
            Serial.println("Still triggered...");
            //print_switch_status();

            Serial.println("Moving Y + to escape");

            yMotor->move(y_escape_steps);
        } else {
            Serial.println("Moving X - worked");
            //print_switch_status();

            return true;
        }

        Serial.println("Result of moving Y + :");
        switches = switch_change();
        print_switch_status(switches);

        if(any_limit()) {
            Serial.println("Still triggered...");
            //print_switch_status();

            Serial.println("Moving Y - to escape");

            yMotor->move(-y_escape_steps);
        }

        Serial.println("Result of moving Y - :");
        switches = switch_change();
        print_switch_status(switches);

        if(any_limit()) {
            Serial.println("Give up?");

            return false;
        }
    } else {
        Serial.println("No blockages");

        return true;
    }
}

bool detect_motors(void) {
    if(!any_limit()) {
        while(!any_limit()) {
            xMotor->step();
        }
    } else {
        clear_unknown_blockage();
    }

    if(x_limit()) {
        // It's the correct axis
        Serial.println("  - Orientation Correct.");

        return true;
    } else {
        // Incorrect axis
        Serial.println("  - Orientation Incorrect.");

        swap_motors();

        return false;
    }
}

bool _triggered_limit(void) {
    Serial.println("bool (*triggered_limit)(void) pointer wasn't set...");
}

void free_axis(Motor *motor, bool (*pos_limit)(void), bool (*neg_limit)(void)) {
    Serial.println("  - Freeing Axis");

    bool (*triggered_limit)(void) = &_triggered_limit;

    if(pos_limit()) {
        Serial.println("  - Positive");
        motor->set_direction(Motor::Forward);
        triggered_limit = pos_limit;
    }

    if(neg_limit()) {
        Serial.println("  - Negative");
        motor->set_direction(Motor::Backward);
        triggered_limit = neg_limit;
    }

    for(int i = 0; i < 100; i++) {
        motor->step();
    }

    if(triggered_limit()) {
        Serial.println("  - Stuck?");
    } else {
        Serial.println("  - Freed!");
    }
}

void calibration(void) {
    Serial.println("Calibration beginning.");

    xMotor->set_direction(Motor::Forward);
    yMotor->set_direction(Motor::Forward);

    xMotor->set_speed(250);
    yMotor->set_speed(250);

    bool x_flipped = false;
    bool y_flipped = false;

    bool motors_flipped = false;

    long x_distance = 0L;
    long y_distance = 0L;

    /*while(y_neg_limit()) {
        yMotor->step();
        y_distance++;
    }

    Serial.println(y_distance, DEC);

    while(x_pos_limit()) {
        xMotor->step();
        x_distance++;
    }

    Serial.println(x_distance, DEC);*/

    // Find an initial home position.

    Serial.println("  - Determining motor orientation.");

    clear_unknown_blockage();

    //motors_flipped = detect_motors();

    return;

    Serial.println("Calibration procedure completed:");
    Serial.println(x_flipped);
    Serial.println(y_flipped);
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
