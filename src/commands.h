#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <Arduino.h>

#include "Motor.h"

void acc(void);
void write_long_command(void);
void read_long_command(void);
void read_setting_command(void);
void write_setting_command(void);
void speed_command(void);
void home_command(void);
void move_command(void);
Motor * motor_from_axis(unsigned const char axis);
void move(unsigned const char axis, long steps);
void power_command(void);
void lower_command(void);
void pause_command(void);
void resume_command(void);
void print_command(void);
void print_ram(void);

#endif
