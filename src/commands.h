#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <Arduino.h>
#include <SD.h>

#include "Motor.h"

void acc(void);
void read_setting_command(void);
void read_saved_setting_command(void);
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
void ls(void);
bool is_printer_file(File file);
void help_command(void);
void calibrate_command(void);
void calibrate_loop_command(void);
void motors_off_command(void);
void motors_on_command(void);

void zero_position_command(void);
void goto_zero_command(void);
void current_position_command(void);

void continuous_move(void);

void init_sd_command(void);

void limit_switch_command(void);

void analog_command(void);
void digital_command(void);

void red_command(void);
void green_command(void);
void blue_command(void);
void pwm_command(void);

#endif
