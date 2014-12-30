#ifndef _ROLLERS_H_
#define _ROLLERS_H_

#include <Servo.h>
#define ROLLER_SERVO_PIN      10	// AOUT 1

class Rollers {
public:
    Rollers();
    ~Rollers();

    void enable(void);
    void disable(void);

    void deploy(void);
    void retract(void);

		unsigned char getangle(void);
    void angle(unsigned char angle);
    void setrp(unsigned char angle);
    void setdp(unsigned char angle);

    static unsigned int width_with_overlap(double overlap);

    static const int roller_width = 1280;
    static const int roller_spacing = 500;

private:
    bool deployed;

    int retracted_position; // These can not be static const, as they need to be set from gui.py
    int deployed_position;

    Servo roller_servo;
};

#endif
