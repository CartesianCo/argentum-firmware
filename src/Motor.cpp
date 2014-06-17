//
//  Motor.cpp
//
//
//  Created by Isabella Stephens on 10/12/12.
//  Copyright (c) 2012 Cartesian Co. All rights reserved.
//

#include "Motor.h"

Motor::Motor(int stepPin, int dirPin, int powerPin, int stepsPerRev) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->powerPin = powerPin;
    this->stepsPerRev = stepsPerRev;
    this->direction = 0; //0 is forward 1 is backwards
    this->position = 0;
    this->lastStepTime = 0;

    /* Speed is the delay between steps necessary to move at the required speed
     * The default speed is 500. To move instantaneously, set speed to 0 (this
     * will cause the motors to skip). Speed is not used by functions which step
     * only once.
     */
    setSpeed(1000);

    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(powerPin, OUTPUT);
    digitalWrite(stepPin, LOW);
    digitalWrite(dirPin, LOW);
    digitalWrite(powerPin, HIGH);
}

Motor::Motor() {
    setSpeed(500);
}

void Motor::power(int state) {
    if(state) digitalWrite(powerPin, LOW);
    else digitalWrite(powerPin, HIGH);
}

long Motor::getPosition() {
    return position;
}

void Motor::resetPosition() {
    int currentDirection = this->direction;
    /*
    if (position > 0) {
        setDir(1);
        move(position);
    } else if (position < 0) {
        setDir(0);
        move(position*-1);
    }*/
    move(position);
    //power(0);

    setDir(currentDirection);
}

void Motor::setDir(int direction) {
    this->direction = direction;
    if(direction) digitalWrite(dirPin, HIGH);
    else digitalWrite(dirPin, LOW);
}

void Motor::step(int direction) {
    digitalWrite(dirPin, direction);
    this->direction = direction;
    step();
}

void Motor::step() {
    while((micros() - lastStepTime) < speed) {
        if(micros() < lastStepTime) {
            delay(speed);
            continue;
        }
    }
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);

    lastStepTime = micros();

    if(direction) position--;
    else position++;
}

void Motor::move(long steps) {
    power(1);
    long distance = steps;

    if(steps < 0) {
        setDir(0);
        distance = steps * -1;
    }
    else setDir(1);

    for (long i = 0; i < distance; i++) {
        step();
    }
}

void Motor::moveUM(long um) {
    long steps = (um*stepsPerMeter)/1000000L;
    move(steps);
}

/*
 * This function moves by nozzle widths. Offset is to 1 decimal place
 * so 10 will move 1 nozzle width, etc.
 */
void Motor::moveNozzleOffset(int offset) {
    //85 um is a nozzle width
    //long um = (offset*265L)/10L;
    //long um = (offset*847L)/10L;
    //long um = (offset*1693L)/10L;
    long um = (offset*169L)/10L;
    moveUM(um);

}

void Motor::setSpeed(int mmPerMinute) {
    int rate = mmPerMinute;
    if (rate > 5000) rate = 5000; //maximum rate
    else if (rate < 0) rate = 0; // cannot have a negative rate

    long i = 1000000000/((rate * stepsPerMeter)/60);
    this->speed = i;
}

int Motor::getSpeed() {
    return this->speed;
}
