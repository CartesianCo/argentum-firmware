#include "commands.h"

#include "AccelStepper.h"
#include "Motor.h"
#include "SerialCommand.h"
#include "settings.h"
#include "limit_switch.h"
#include "utils.h"
#include "calibration.h"
#include "LEDStrip.h"
#include "rollers.h"
#include <Servo.h>
#include <SD.h>

#include "comms.h"

#include "Axis.h"

#include "logging.h"

#include "argentum.h"

extern bool readFile(char *filename);
extern void file_stats(char *filename);

extern Motor *xMotor;
extern Motor *yMotor;

void forward(void) {
    xMotor->move(1);
}

void backward(void) {
    xMotor->move(-1);
}

void fforward(void) {
    yMotor->move(1);
}

void bbackward(void) {
    yMotor->move(-1);
}

void acc(void) {
    AccelStepper as(&forward, &backward);
    AccelStepper bs(&fforward, &bbackward);

    as.setMaxSpeed(50000);
    as.setAcceleration(5000);

    bs.setMaxSpeed(50000);
    bs.setAcceleration(5000);

    bool as_end = false;
    bool bs_end = false;

    while(true) {
        if(!as.run()) {
            if(as_end) {
                as.moveTo(0);
                as_end = false;
            } else {
                as.moveTo(5000);
                as_end = true;
            }
        }

        if(!bs.run()) {
            if(bs_end) {
                bs.moveTo(0);
                bs_end = false;
            } else {
                bs.moveTo(5000);
                bs_end = true;
            }
        }

        if(Serial.available()) {
            return;
        }
    }
}

void motors_off_command(void) {
    Serial.println("Motors off");
    xMotor->power(0);
    yMotor->power(0);
}

void motors_on_command(void) {
    Serial.println("Motors on");
    xMotor->power(1);
    yMotor->power(1);
}

void read_setting_command(void) {
    Serial.println("Current Global Settings:");
    settings_print_settings(&global_settings);
}

void read_saved_setting_command(void) {
    PrinterSettings settings;

    settings_read_settings(&settings);

    Serial.println("EEPROM Settings:");
    settings_print_settings(&settings);
}

void write_setting_command(void) {
    settings_write_settings(&global_settings);
}

void speed_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing speed parameter");
        return;
    }

    long speed = atol(arg);

    if(speed <= 0) {
        speed = 1;
    }

    Motor *motor = motor_from_axis(axis);
    motor->set_speed(speed);
}

void zero_position_command(void) {
    logger.info("Setting new zero position");
    xMotor->set_position(0L);
    yMotor->set_position(0L);

    x_axis.zero();
    y_axis.zero();
}

void goto_zero_command(void) {
    logger.info("Returning to 0.000, 0.000");
    xMotor->go_home();
    yMotor->go_home();

    x_axis.move_absolute(0.00);
    y_axis.move_absolute(0.00);
}

void current_position_command(void) {
    Serial.print("Current position (steps): (");
    Serial.print(xMotor->get_position());
    Serial.print(", ");
    Serial.print(yMotor->get_position());
    Serial.println(")");

    logger.info() << "X: " << x_axis.get_current_position() << " mm, "
            << "Y: " << y_axis.get_current_position() << " mm"
            << Comms::endl;
}

void home_command(void) {
    while(!neg_limit()) {
        xMotor->move(-1);
        yMotor->move(-1);
    }

    while(!x_neg_limit()) {
        xMotor->move(-1);
    }

    while(!y_neg_limit()) {
        yMotor->move(-1);
    }
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

Motor * motor_from_axis(unsigned const char axis) {
    if (toupper(axis) == 'X') {
        return xMotor;
    } else if (toupper(axis) == 'Y') {
        return yMotor;
    }

    return NULL;
}

void continuous_move(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing direction parameter");
        return;
    }

    char direction = arg[0];

    Motor *motor = motor_from_axis(axis);

    while(!Serial.read() == 'c') {
        if(direction == '+') {
            motor->move(1);
        } else {
            motor->move(-1);
        }
    }
}

void move(const char axis, long steps) {
    Motor *motor = motor_from_axis(axis);

    if(!motor) {
        logger.error() << "Cannot obtain motor pointer for " << axis << "axis"
                << Comms::endl;
        return;
    }

    //logger.info() << "Moving " << axis << " axis " << steps << "steps" << Comms::endl;

    if(steps == 0) {
        //motor->reset_position();
        motor->go_home();
    } else {
        motor->move(steps);
    }
}

void power_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing power parameter");
        return;
    }

    char power = arg[0];

    Motor *motor = NULL;

    if(toupper(axis) == 'X') {
        motor = xMotor;
    } else if(toupper(axis) == 'Y') {
        motor = yMotor;
    } else {
        logger.error("No axis");
        return;
    }

    if(power == '0') {
        motor->power(0);
    } else if (power == '1') {
        motor->power(1);
    } else {
        logger.error("Unknown power");
        return;
    }
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
        arg = "output.hex";
    }

    strcpy(filename, arg);

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

void stat_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.info("No filename supplied, using 'output.hex'");
        arg = "output.hex";
    }

    logger.info() << "Statting '" << arg << "'" << Comms::endl;

    file_stats(arg);
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
    File root = SD.open("/.");

    File file = root.openNextFile();

    while(file) {
        if(is_printer_file(file)) {
            logger.info() << file.name() << Comms::endl;
        }

        file.close();

        file = root.openNextFile();
    }

    root.close();
}

bool is_printer_file(File file) {
    char *file_name = file.name();

    if(strstr(file_name, ".HEX")) {
        return true;
    }

    return false;
}

void help_command(void) {
    comms.println("Press p to print output.hex");
    comms.println("S to stop, P to pause, R to resume, c to calibrate.");
    comms.println("Additional commands: ");
    serial_command.installed_commands();
    comms.println();

    comms.println("Available Files: ");
    ls();
    comms.println();
}

void calibrate_command(void) {
    CalibrationData calibration;
    calibrate(&calibration);

    settings_print_calibration(&calibration);
    settings_update_calibration(&calibration);

    /*log_info("C ");
    settings_print_axis_data_minimal(&calibration.x_axis);
    log_info_np(" ");
    settings_print_axis_data_minimal(&calibration.y_axis);
    log_info("\r\n");*/

    /*logger.info() << settings_print_axis_data_minimal(&calibration.x_axis)
            << " " << settings_print_axis_data_minimal(&calibration.y_axis)
            << Comms::endl;*/
}

void calibrate_loop_command(void) {
    while(true) {
        if(Serial.available()) {
            if(Serial.read() == 'S') {
                return;
            }
        }

        CalibrationData calibration;
        calibrate(&calibration);

        logger.info() << calibration.x_axis.motor << " "
                << calibration.x_axis.length << ", " << calibration.x_axis.motor
                << " " << calibration.x_axis.length << Comms::endl;
    }
}

void init_sd_command(void) {
    if(!SD.begin(53)) {
        logger.warn("Failed to initialise SD card.");
    }
}

void limit_switch_command(void) {
    print_switch_status();
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

    setRed(value);
}

void green_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value (0 .. 255) supplied");
        return;
    }

    int value = atoi(arg);

    setGreen(value);
}

void blue_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("No value (0 .. 255) supplied");
        return;
    }

    int value = atoi(arg);

    setBlue(value);
}

void rollers_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        logger.error("Missing position argument");
        return;
    }

    int value = atoi(arg);

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
        move('x', width);
        rollers.retract();

        move('x', -width);
        rollers.deploy();

        move('x', width);
        rollers.retract();

        move('x', -width);

        // 3. Raise rollers
        rollers.retract();
        delay(100);

        // 4. Advance Y position
        move('y', delta_y);
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

void axis_pos(void) {
    logger.info() << "X: " << x_axis.get_current_position() << " mm, "
            << "Y: " << y_axis.get_current_position() << " mm"
            << Comms::endl;
}

void size_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing x size");
        logger.info() << "Current size: " << x_size << " x " << y_size
                << Comms::endl;
        return;
    }

    x_size = atol(arg);

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing y size");
        return;
    }

    y_size = atol(arg);
}
