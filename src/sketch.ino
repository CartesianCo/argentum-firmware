#include "Motor.h"
#include "LEDStrip.h"
#include <SD.h>
#include <Servo.h>
#include "settings.h"

Motor aMotor(15, 14, 16, 0); // X
Motor bMotor(18, 17, 19, 0); // Y

Motor *xMotor = &aMotor;
Motor *yMotor = &bMotor;

int accessory = 39;

Servo ServoR; // The right drying roller servo
Servo ServoL; // The left drying roller servo

File myFile;

void setup() {
    Serial.begin(9600);
    Serial.flush();

    pinMode(accessory, OUTPUT);
    digitalWrite(accessory, HIGH);

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

    ServoR.attach(14);
    ServoL.attach(15);
    ServoR.write(20);
    ServoL.write(45);

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
                while(Serial.peek() == -1){}
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
            //xMotor.setSpeed(3000);
            //xMotor.setSpeed(3000);
            for (int i=0; i <= 0; i++) readFile("Output.hex");

            break;

        case 'P': // P
            Serial.println("Paused - enter R to resume");
            while(Serial.read() != 'R'){}

            Serial.println("Resuming");

            break;

        case 'm':
        case 'M':
            char cmdInt[5];

            for(int i = 0; i < 5; i++) {
                cmdInt[i] = command[4+i];
            }

            dist = atoi(cmdInt);

            if (toupper(command[2]) == 'X') {
                if(dist == 0) {
                    xMotor->reset_position();
                } else {
                    xMotor->move(-dist);
                }
            } else if (toupper(command[2]) == 'Y') {
                if(dist == 0) {
                    yMotor->reset_position();
                } else {
                    yMotor->move(-dist);
                }
            }

            break;

        case 'C':
            calibration();

            break;

        case '?':
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

#define x_pos_limit() digitalRead(x_pos_limit_pin)
#define x_neg_limit() digitalRead(x_neg_limit_pin)

#define y_pos_limit() digitalRead(y_pos_limit_pin)
#define y_neg_limit() digitalRead(y_neg_limit_pin)

#define x_limit() (x_pos_limit() || x_neg_limit())
#define y_limit() (y_pos_limit() || y_neg_limit())

#define any_limit() (x_limit() || y_limit())

void calibration(void) {
    /*while(1) {
        if(x_pos_limit()) {
            Serial.println("X - POS");
        }

        if(x_neg_limit()) {
            Serial.println("X - NEG");
        }

        if(y_pos_limit()) {
            Serial.println("Y - POS");
        }

        if(y_neg_limit()) {
            Serial.println("Y - NEG");
        }
    }*/
    Serial.println("Calibration beginning.");

    /*

    Procedure:
        - Attempt to home the head, we don't know which way is which at this
          point, however we are assuming that the limit switches are wired
          correctly.

        - Command both axes to move in the positive direction, and monitor the
          limit switch inputs for collisions.

        - Next, move in the opposite direction until a collision, and count
          the number of steps required. This is the bed size.

    */

    bool x_flipped = false;
    bool y_flipped = false;

    bool motors_flipped = false;

    long x_distance = 0L;
    long y_distance = 0L;

    // Find an initial home position.

    Serial.println("? - Initial Maximum (either, should be X)");

    while(!any_limit()) {
        xMotor->move(-1);
    }

    if(x_limit()) {
        // Motor axes are correct
        Serial.println("X - Found First Limit");

        if(x_neg_limit()) {
            Serial.println("X - Flipping Motor Direction");
            x_flipped = true;
        }

        Serial.println("X - Finding Second Limit");
        while(!x_limit()) {
            xMotor->move(1);

            x_distance++;
        }
    }

    if(y_limit()) {
        // Motor axes are not correct
        Serial.println("Y - Found First Limit [ MOTOR CONNECTION ]");

        if(y_neg_limit()) {
            Serial.println("Y - Flipping Motor Direction");
            y_flipped = true;
        }

        motors_flipped = true;
        xMotor = &bMotor;
        yMotor = &aMotor;

        Serial.println("Y - Finding Second Limit");

        while(!y_limit()) {
            yMotor->move(1);

            y_distance++;
        }
    }

    //goto complete;

    Serial.println("Y - Initial Maximum (either)");

    while(!y_limit()) {
        yMotor->move(-1);
    }

    Serial.println("Y - Found First Limit");


complete:

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
