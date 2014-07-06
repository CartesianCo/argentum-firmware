#include "commands.h"

#include "AccelStepper.h"
#include "Motor.h"
#include "SerialCommand.h"
#include "settings.h"
#include "limit_switch.h"
#include "utils.h"
#include "calibration.h"

extern void readFile(char* filename);

extern SerialCommand serial_command;

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
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        Serial.println("Missing steps parameter");
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
}

void move(unsigned const char axis, long steps) {
    Motor *motor = motor_from_axis(axis);

    if(steps == 0) {
        motor->reset_position();
    } else {
        motor->move(steps);
    }
}

void power_command(void) {
    char *arg;

    arg = serial_command.next();

    if(!arg) {
        Serial.println("Missing axis parameter");
        return;
    }

    char axis = arg[0];

    arg = serial_command.next();

    if(!arg) {
        Serial.println("Missing power parameter");
        return;
    }

    char power = arg[0];

    Motor *motor = NULL;

    if(toupper(axis) == 'X') {
        motor = xMotor;
    } else if(toupper(axis) == 'Y') {
        motor = yMotor;
    } else {
        Serial.println("No axis");
        return;
    }

    if(power == '0') {
        motor->power(0);
    } else if (power == '1') {
        motor->power(1);
    } else {
        Serial.println("Unknown power");
        return;
    }
}

void lower_command(void) {
    Serial.println("Lower/Raise");

    // If it is already raised, lower it
    /*if (ServoR.read() == 45) {
        ServoR.write(20); //Raised
        ServoL.write(45); //Left servo is opposite

        Serial.println("Raise");

        return;
    }

    // If it is lowered, raise it
    if (ServoR.read() == 20) {
        ServoR.write(45); //Raised
        ServoL.write(20); //Left servo is opposite

        Serial.println("Lower");

        return;
    }*/
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

    arg = serial_command.next();

    if(!arg) {
        Serial.println("No filename supplied, using 'output.hex'");
        arg = "output.hex";
    }

    Serial.print("Printing ");
    Serial.println(arg);

    readFile(arg);
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
    File root = SD.open("/");

    File file = root.openNextFile();

    while(file) {
        if(is_printer_file(file)) {
            Serial.println(file.name());
        }

        file.close();

        file = root.openNextFile();
    }
}

bool is_printer_file(File file) {
    char *file_name = file.name();

    if(strstr(file_name, ".HEX")) {
        return true;
    }

    return false;
}

void help_command(void) {
    Serial.println("Press p to print output.hex");
    Serial.println("S to stop, P to pause, R to resume, c to calibrate.");
    Serial.println("Additional commands: ");
    serial_command.installed_commands();
    Serial.println();

    Serial.println("Available Files: ");
    ls();
    Serial.println();
}

void calibrate_command(void) {
    CalibrationData calibration;
    calibrate(&calibration);

    settings_update_calibration(&calibration);
}
