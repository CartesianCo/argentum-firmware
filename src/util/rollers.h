#ifndef _ROLLERS_H_
#define _ROLLERS_H_

#include <Servo.h>

class Rollers {
public:
    Rollers();
    ~Rollers();

    void enable(void);
    void disable(void);

    void deploy(void);
    void retract(void);

    void angle(unsigned char angle);

    static unsigned int width_with_overlap(double overlap);

    static const int roller_width = 1280;
    static const int roller_spacing = 500;

private:
    bool deployed;

    Servo roller_servo;
    static const int roller_servo_pin = 11;

    static const int retracted_position = 55;
    static const int deployed_position = 80;

};

#endif
