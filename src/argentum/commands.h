#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <Arduino.h>
#include <SD.h>

#include "../util/stepper.h"
#include "../util/axis.h"

void acc(void);
void read_setting_command(void);
void read_saved_setting_command(void);
void write_setting_command(void);
void speed_command(void);
void home_command(void);
void move_command(void);

Stepper * motor_from_axis(unsigned const char axis);
Axis * axis_from_id(uint8_t id);

void move(const char axis, long steps);
void power_command(void);

void lower_command(void);
void rollers_command(void);

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

void sweep_command(void);
void sweep(long width, long height);

void absolute_move(void);
void incremental_move(void);
void axis_pos(void);

void stat_command(void);

void size_command(void);

void plus_command(void);
void minus_command(void);
void wait_command(void);

void primitive_voltage_command(void);

#endif
