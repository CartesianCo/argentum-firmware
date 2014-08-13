#include "commands.h"

//#include "AccelStepper.h"
#include "../util/SerialCommand.h"
#include "../util/settings.h"
#include "../util/limit.h"
#include "../util/utils.h"
#include "calibration.h"
#include "../util/colour.h"
#include "../util/rollers.h"
#include "../util/comms.h"
#include "../util/axis.h"
#include "../util/logging.h"
#include "../util/codes.h"

#include "boardtests.h"

#include "argentum.h"

extern bool readFile(char *filename);

long xpos = 0;
long ypos = 0;

void motors_off_command(void) {
    x_axis.get_motor()->enable(false);
    y_axis.get_motor()->enable(false);

    logger.info("Motors off");
}

void motors_on_command(void) {
    x_axis.get_motor()->enable(true);
    y_axis.get_motor()->enable(true);

    logger.info("Motors on");
}

void read_setting_command(void) {
    settings_print_settings(&global_settings);
}

void read_saved_setting_command(void) {
    PrinterSettings settings;

    settings_read_settings(&settings);

    settings_print_settings(&settings);
}

void write_setting_command(void) {
    settings_write_settings(&global_settings);
}

void speed_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing speed parameter");
        return;
    }

    long speed = atol(arg);

    if(speed <= 0) {
        speed = 1;
    }

    Stepper *motor = motor_from_axis(axis);
    motor->set_speed(speed);
}

void zero_position_command(void) {
    logger.info("Setting new zero position");

    x_axis.zero();
    y_axis.zero();
}

void goto_zero_command(void) {
    logger.info("Returning to 0.000, 0.000");

    x_axis.move_absolute(0.000);
    y_axis.move_absolute(0.000);
}

void current_position_command(void) {
    logger.info() << x_axis.get_current_position() << ","
            << y_axis.get_current_position() << Comms::endl;
}

void move_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing steps parameter");
        return;
    }

    long steps = atol(arg);

    move(axis, steps);
}

void continuous_move(void) {
    logger.error("Not implemented.");
}

void move(const char axis_id, long steps) {
    Axis *axis = axis_from_id(axis_id);

    if(!axis) {
        logger.error() << "Cannot obtain pointer for " << axis_id << "axis"
                << Comms::endl;
        return;
    }

    if(steps == 0) {
        axis->move_absolute(0.000);
    } else {
        axis->move_incremental(steps);
    }

    axis->wait_for_move();
}

void lower_command(void) {
    logger.info("Lower/Raise");

    rollers.deploy();
}

void pause_command(void) {
    Serial.println("Paused - enter R to resume");
    while(Serial.read() != 'R');

    Serial.println("Resuming");
}

void resume_command(void) {
    Serial.println("Resuming");
}

void print_command(void) {
    char *arg;

    static char filename[32] = "output.hex";
    uint8_t passes = 1;

    // TODO: Should I make a copy of the file name below?
    arg = serial_command.next();

    if(!arg) {
        logger.info("No filename supplied, using 'output.hex'");
    } else {
        strcpy(filename, arg);
    }

    arg = serial_command.next();

    if(arg) {
        passes = atoi(arg);
    }

    logger.info() << "Printing '" << filename << "'" << Comms::endl;

    for(int pass = 0; pass < passes; pass++) {
        logger.info() << "Pass " << (pass + 1) << " of " << passes << Comms::endl;

        bool result = readFile(filename);

        if(0) {
            long x_delta = 6500 + x_size;

            logger.info() << "x_delta: " << x_delta << Comms::endl;
            logger.info() << -6500 - x_size << Comms::endl;

            move('X', -6500 - x_size);
            move('Y', -500);

            // TODO: Should get these params from the readFile (or print) function
            // Perhaps passing in some kind of printinfo struct containing print
            // statistics after it's done.
            sweep(x_size + 2000, y_size + 1000);

            move('X', x_delta);
            move('Y', 500);
        } else {
            logger.info("Something went wrong in readFile, aborting print.");
            return;
        }
    }

    logger.info("Print complete. Enjoy your circuit!");
}

void print_ram(void) {
    uint16_t used = ram_used();
    double utilisation = ram_utilisation();

    Serial.print("Using ");
    Serial.print(used);
    Serial.print(" bytes out of 8192 (");
    Serial.print(utilisation);
    Serial.println("%)");
}

void ls(void) {
    SdFile file;
    char name[13];

    sd.vwd()->rewind();

    uint16_t count = 0;

    while (file.openNext(sd.vwd(), O_READ)) {
        file.getFilename(name);

        if(strstr(name, ".HEX")) {
            count++;
        }

        file.close();
    }

    comms.send("*");
    comms.send(count);
    comms.send(Comms::endl);

    sd.vwd()->rewind();

    while (file.openNext(sd.vwd(), O_READ)) {
        file.getFilename(name);

        if(strstr(name, ".HEX")) {
            logger.info(name);
        }

        file.close();
    }
}

void help_command(void) {
    logger.info("Press p to print output.hex");
    logger.info("S to stop, P to pause, R to resume, c to calibrate.");
    logger.info("Additional commands: ");
    serial_command.installed_commands();
    logger.info();

    logger.info("Available Files: ");
    ls();
    logger.info();
}

void calibrate_command(void) {
    CalibrationData calibration;
    calibrate(&calibration);

    settings_update_calibration(&calibration);
}

void init_sd_command(void) {
    if(!sd.begin(53, SPI_HALF_SPEED)) {
        logger.warn("Failed to initialise SD card.");
    } else {
        logger.info(code_strings[CODE_SUCCESS]);
    }
}

void limit_switch_command(void) {
    print_switch_status();

    x_axis.debug_info();
    y_axis.debug_info();
}

void analog_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No pin supplied");
        return;
    }

    int pin = atoi(arg);

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value supplied");
        return;
    }

    int value = atoi(arg);

    logger.info() << "Setting " << pin << " to " << value << Comms::endl;

    analogWrite(pin, value);
}

void digital_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No pin supplied");
        return;
    }

    int pin = atoi(arg);

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value supplied");
        return;
    }

    int value = atoi(arg);

    logger.info() << "Setting " << pin << " to " << value << Comms::endl;

    digitalWrite(pin, value);
}

void red_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value (0 .. 255) supplied");
        return;
    }

    int value = atoi(arg);

    colour_red(value);
}

void green_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value (0 .. 255) supplied");
        return;
    }

    int value = atoi(arg);

    colour_green(value);
}

void blue_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value (0 .. 255) supplied");
        return;
    }

    int value = atoi(arg);

    colour_blue(value);
}

void rollers_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing position argument");
        return;
    }

    int value = atoi(arg);

    //rollers.angle(value);

    //return;

    if(value == 0) {
        logger.info("Retracting");
        rollers.retract();
    } else {
        logger.info("Deploying");
        rollers.deploy();
    }
}

void pwm_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No pin supplied");
        logger.info("Valid options are 7, 8, and 9");
        return;
    }

    int pin = atoi(arg);

    if(pin < 7 || pin > 9) {
        logger.error("Hey... That's not a PWM pin!");
        return;
    }

    arg = serial_command.next();

    if(!arg) {
        logger.error("No PWM value supplied");
        return;
    }

    int value = atoi(arg);

    if(value < 0 || value > 255) {
        logger.error("Value out of range (0:255)");
    }

    analogWrite(pin, value);
}

void sweep_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing width");
        return;
    }

    int width = atoi(arg);

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing height");
        return;
    }

    int height = atoi(arg);

    sweep(width, height);
}

void sweep(long width, long height) {
    // 1. Calculate number of passes and pass height

    unsigned int pass_width = rollers.width_with_overlap(0.5) / 2;

    unsigned int passes =  ceil(height / pass_width);
    unsigned int delta_y = floor(height / passes);

    logger.info("Sweeping.");
    logger.info() << "   Coverage per pass: " << pass_width << Comms::endl;
    logger.info() << "   Total Width: " << width << Comms::endl;
    logger.info() << "   Total Height: " << height << Comms::endl;
    logger.info() << "   Passes: " << passes << ", Delta Y: " << delta_y
            << Comms::endl;

    for(int pass = 0; pass < passes; pass++) {
        logger.info() << "   Pass: " << pass + 1 << " of " << passes << Comms::endl;

        // 1. Lower rollers
        rollers.deploy();
        delay(100);

        // 2. Make first x pass
        move('y', width);
        rollers.retract();

        move('y', -width);
        rollers.deploy();

        move('y', width);
        rollers.retract();

        move('y', -width);

        // 3. Raise rollers
        rollers.retract();
        delay(100);

        // 4. Advance Y position
        move('x', delta_y);
    }

    int y_offset = delta_y * passes;

    move('y', -y_offset);
}

void absolute_move(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing x position (double)");
        return;
    }

    double x_position = atof(arg);

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing y position (double)");
        return;
    }

    double y_position = atof(arg);

    if(x_position < 0 || y_position < 0) {
        logger.error("Absolute positions must be positive.");
        return;
    }

    x_axis.move_absolute(x_position);
    y_axis.move_absolute(y_position);
}

void incremental_move(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing x position (double)");
        return;
    }

    double x_position = atof(arg);

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing y position (double)");
        return;
    }

    double y_position = atof(arg);

    x_axis.move_incremental(x_position);
    y_axis.move_incremental(y_position);
}

void plus_command(void) {
    x_axis.move_to_positive();
}

void minus_command(void) {
    x_axis.move_to_negative();
}

void wait_command(void) {
    x_axis.wait_for_move();
}

void primitive_voltage_command(void) {
    double voltage = primitive_voltage();

    logger.info(voltage);
}
