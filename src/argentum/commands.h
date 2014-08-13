#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <Arduino.h>
//#include <SD.h>

#include "../util/stepper.h"
#include "../util/axis.h"
#include "../util/SdFat/SdFat.h"

void read_setting_command(void);
void read_saved_setting_command(void);
void write_setting_command(void);

// Motors
void speed_command(void);
void move_command(void);

void motors_off_command(void);
void motors_on_command(void);

void zero_position_command(void);
void goto_zero_command(void);
void current_position_command(void);

void continuous_move(void);

void move(const char axis, long steps);

void absolute_move(void);
void incremental_move(void);
void axis_pos(void);

void plus_command(void);
void minus_command(void);
void wait_command(void); // Is this necessary?

// Rollers
void lower_command(void);
void rollers_command(void); // Delete one of these

void sweep_command(void); // Delete these, sweeping should be implemented in Python
void sweep(long width, long height); // ^^^^^

// Printing
void pause_command(void);
void resume_command(void);
void print_command(void);
void print_ram(void);

void help_command(void);
void calibrate_command(void);
void calibrate_loop_command(void);

// SD
void init_sd_command(void);
void ls(void);

// GPIO
void analog_command(void);
void digital_command(void);

void red_command(void);
void green_command(void);
void blue_command(void);
void pwm_command(void);

void limit_switch_command(void);

void primitive_voltage_command(void);

#endif
