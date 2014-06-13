#include "Motor.h"
#include <SD.h>
#include <Servo.h>

Motor yMotor(A14, A13, A15, 0);
Motor xMotor(A11, A10, A12, 0);

//Motor yMotor(3, 2, 4, 0);
//Motor xMotor(39, 41, 44, 0);
//Motor xMotor(3, 2, 4, 0);
//Motor yMotor(39, 41, 44, 0);

int acessory = 39;

Servo ServoR; // The right drying roller servo
Servo ServoL; // The left drying roller servo

File myFile;

void setup()
{
  //pinMode(53, OUTPUT); // Setup pin for the SD Shield
  Serial.begin(9600); //Initalise Serial
  Serial.flush();

  pinMode(acessory, OUTPUT);
  digitalWrite(acessory, HIGH);

  DDRC = 255;
  DDRL = 255;
  DDRA = 255;

  ServoR.attach(14);
  ServoL.attach(15);
  ServoR.write(20);
  ServoL.write(45);



  delay(1000);
  if(!SD.begin(53)) { //Initalise SD
    Serial.println("SD card could not be accessed.");
  }

  Serial.println("Press p to print Output.txt, S to stop, P to pause, R to resume");
  //xMotor.setSpeed(5000);
}

void loop()
{

}


void serialEvent()
{
  byte command[10];
  int i;

  // Loop through all serial data
  while (Serial.available()) {

    // read in first byte of command
    command[0] = Serial.read();

    // read in extra bytes if necessary
    switch(command[0]) 
    {
      case 112:
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


void parseCommand(byte* command)
{
  int dist = 0;

  switch(command[0])
  {
    case 1:
      fireHead((byte)command[1], (byte)command[2], (byte)command[5], (byte)command[6]);
      /*
      Serial.print("C: ");
      Serial.print((byte)command[1]);
      Serial.print(" ");
      Serial.println((byte)command[5]);
      */
      break;

   case 2:
      xMotor.setDir(1);
      xMotor.move(-1);
      //Serial.println("Move left");
      break;

    case 3:
      xMotor.setDir(0);
      xMotor.move(1);
      break;

    case 4:
      yMotor.setDir(1);
      yMotor.move(-1);
      break;

    case 5:
      yMotor.setDir(0);
      yMotor.move(1);
      break;


    case 7:
     for (int i=0; i <= command[1]; i++) delay(5000);
     break;

    case 8:
      //digitalWrite(acessory, !digitalRead(acessory));
      break;

    case 112: // p
      Serial.println("Printing file.");
      xMotor.setSpeed(3000);
      xMotor.setSpeed(3000);
      for (int i=0; i <= 0; i++) readFile("Output.hex");
      break;

    case 80: // P
      Serial.println("Paused - enter R to resume");
      while(Serial.read() != 'R'){}
      break;

    case 'm':
    case 'M':
      char cmdInt[5];
      for(int i = 0; i < 5; i++) cmdInt[i] = command[4+i];
      dist = atoi(cmdInt);
      if (toupper(command[2]) == 'X') {
        if(dist == 0) xMotor.resetPosition();
        else xMotor.move(-2*dist);
      } else if (toupper(command[2]) == 'Y') {
        if(dist == 0) yMotor.resetPosition();
        else yMotor.move(-1*dist);
      }
      break;

    case 'f':
    case 'F':
    for(int j = 0; j < 1000; j++) {
        for(int i = 0; i < 13; i++) {
        fireHead(255, i+1, 255, i+1);
        }
        delay(20);
      }

      break;

    case 'x':
    case 'X':
      if(command[1] == '0') xMotor.power(0);
      else if (command[1] == '1') xMotor.power(1);
      else Serial.println(command[1]);
      break;
    case 'y':
    case 'Y':
      if(command[1] == '0') yMotor.power(0);
      else if (command[1] == '1') yMotor.power(1);
      else Serial.println(command[1]);
      break;
    case 'l':
      Serial.println("Lower/Raise");
      if (ServoR.read() == 45) //If it is raised, Lower it
      {
        ServoR.write(20); //Raised
        ServoL.write(45); //Left servo is opposite
        Serial.println("Raise");
        break;
      }
      if (ServoR.read() == 20) // If it is lowered, Raise it
      {
        ServoR.write(45); //Raised
        ServoL.write(20); //Left servo is opposite
        Serial.println("Lower");
        break;
      }
      break;
  }
}


void readFile(char* filename)
{
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

  // loop through file
  while(myFile.available())
  {
    // read in first byte of command
    command[0] = myFile.read();

    // read in extra bytes if necessary
    switch(command[0])
    {
      case 1:
        for(int i = 0; i < 7; i++) command[i + 1] = myFile.read();
        break;
      case 7:
        for(int i = 0; i < 1; i++) command[i + 1] = myFile.read();
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
        xMotor.resetPosition();
        yMotor.resetPosition();
        return;
      }
      serialEvent();
    }
  }

  //close file
  myFile.close();
}

// PORT C is [R1, R2, R3, R4, L1, L2, L3, L4] (Multiplexer)
// PORT L is Left (MOSFET Drivers)
// PORT A is Right (MOSFET Drivers)



// Put in to a printhead class
void fireHead(byte rPrim, byte rAddr, byte lPrim, byte lAddr)
{
  int x1, x2;
  if (rPrim+rAddr+lPrim+lAddr > 0)
      {
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
      //delay(5000);
      PORTA = 0;
      PORTL = 0;
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");  
      PORTC = 0;
      //delayMicroseconds(500);
      }
}

