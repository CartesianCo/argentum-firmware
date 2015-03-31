/*
    Argentum Firmware

    Copyright (C) 2013 Isabella Stevens
    Copyright (C) 2014 Michael Shiel
    Copyright (C) 2015 Trent Waddington

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
void acceleration_command(void);
void move_command(void);

Stepper * motor_from_axis(unsigned const char axis);
Axis * axis_from_id(uint8_t id);

void power_command(void); // Delete
void motors_off_command(void);
void motors_on_command(void);

void zero_position_command(void);
void home_command(void);
void goto_zero_command(void);
void current_position_command(void);

void continuous_move(void);

void move(const char axis, long steps);

void absolute_move(void);
void incremental_move(void);

void plus_command(void);
void minus_command(void);
void wait_command(void);

void stest_command(void);

// Rollers
void rollers_command(void); // Picks up an argument from serial after call.

void sweep_command(void); // Delete these, sweeping should be implemented in Python
void sweep(long width, long height); // ^^^^^

// Printing
void pause_command(void);
void resume_command(void);
void fire_command(void);
void draw_command(void);
void print_command(void);
void print_ram(void);

void help_command(void);
void version_command(void);
void printer_number_command(void);
void calibrate_command(void);
void calibrate_loop_command(void);
void unknown_command(const char *cmd);

// SD
void init_sd_command(void);
void ls_command(void);
void rm_command(void);
void md5_command(void);
void djb2_command(void);
void recv_command(void);
void echo_command(void);

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
