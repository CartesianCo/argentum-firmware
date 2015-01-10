#include "commands.h"

//#include "AccelStepper.h"
#include "../util/SerialCommand.h"
#include "../util/settings.h"
#include "../util/limit.h"
#include "../util/utils.h"
#include "calibration.h"
#include "../util/colour.h"
#include "../util/rollers.h"
#include "../util/cartridge.h"
//#include <SD.h>

#include "../util/comms.h"

#include "../util/axis.h"

#include "../util/logging.h"
extern "C" {
#include "../util/md5.h"
#include "../util/decb.h"
}

#include "boardtests.h"

#include "argentum.h"

extern bool readFile(char *filename);
extern void file_stats(char *filename);
void moveTo(long x, long y);
void lineTo(long x, long y, float width);

long xpos = 0;
long ypos = 0;

void motors_off_command(void) {
    Serial.println("Motors off");

    x_axis.get_motor()->enable(false);
    y_axis.get_motor()->enable(false);
}

void motors_on_command(void) {
    Serial.println("Motors on");

    x_axis.get_motor()->enable(true);
    y_axis.get_motor()->enable(true);
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

    Stepper *motor = motor_from_axis(axis);
    motor->set_speed(speed);
}

void zero_position_command(void) {
    logger.info("Setting new zero position");
    //xMotor->set_position(0L);
    //yMotor->set_position(0L);

    x_axis.zero();
    y_axis.zero();
}

void goto_zero_command(void) {
    logger.info("Returning to 0.000, 0.000");

    x_axis.move_absolute(0.000);
    y_axis.move_absolute(0.000);
}

void home_command(void) {
    x_axis.move_to_negative();
    y_axis.move_to_negative();
    x_axis.zero();
    y_axis.zero();
    logger.info("Homed");
}

void current_position_command(void) {
    logger.info() << "X: " << x_axis.get_current_position() << " mm, "
            << "Y: " << y_axis.get_current_position() << " mm"
            << Comms::endl;

    logger.info() << x_axis.length << ", " << y_axis.length << Comms::endl;

    x_axis.debug_info();
    y_axis.debug_info();
}

void move_command(void) {
    char *arg;

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis = arg[0];
    if (axis >= '0' && axis <= '9')
    {
        // assume this is a moveTo command
        long x = atol(arg);
        arg = serial_command.next();
        long y = atol(arg);
        moveTo(x, y);
        logger.info("Ok");
        return;
    }

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing steps parameter");
        return;
    }

    long steps = atol(arg);

    move(axis, steps);
}

void line_command(void) {
    char *arg;

    arg = serial_command.next();
    long x = atol(arg);
    arg = serial_command.next();
    long y = atol(arg);
    arg = serial_command.next();
    float width = atof(arg);

    lineTo(x, y, width);
    logger.info("Ok");
    //test
}

Axis * axis_from_id(uint8_t id) {
    id = toupper(id);

    switch(id) {
        case Axis::X:
            return &x_axis;
            break;

        case Axis::Y:
            return &y_axis;
            break;

        default:
            return NULL;
    }
}

Stepper * motor_from_axis(unsigned const char axis) {
    if (toupper(axis) == 'X') {
        //return xMotor;
        return x_axis.get_motor();
    } else if (toupper(axis) == 'Y') {
        //return yMotor;
        return y_axis.get_motor();
    }

    return NULL;
}

void continuous_move(void) {
    logger.warn("Not implemented.");
}

void move(const char axis_id, long steps) {
    //Stepper *motor = motor_from_axis(axis);
    Axis *axis = axis_from_id(axis_id);

    if(!axis) {
        logger.error() << "Cannot obtain pointer for " << axis_id << " axis"
                << Comms::endl;
        return;
    }

    //logger.info() << "Moving " << axis << " axis " << steps << "steps" << Comms::endl;

    if(steps == 0) {
        //motor->reset_position();
        //axis->move_absolute(0.000);

        if(axis_id == 'X') {
            axis->move_incremental(-xpos);
            xpos = 0;
        } else {
            axis->move_incremental(-ypos);
            ypos = 0;
        }
    } else {
        axis->move_incremental(steps);

        if(axis_id == 'X') {
            xpos += steps;
        } else {
            ypos += steps;
        }
    }

    axis->wait_for_move();
}

void moveTo(long x, long y)
{
    x_axis.move_absolute((uint32_t)x);
    y_axis.move_absolute((uint32_t)y);
    while (x_axis.moving() || y_axis.moving())
    {
        if (x_axis.moving())
            x_axis.run();
        if (y_axis.moving())
            y_axis.run();
    }

    xpos = x;
    ypos = y;
}

void lineTo(long x, long y, float width)
{
    long dx = x - xpos;
    long dy = y - ypos;
    int32_t dirx = 1;
    int32_t diry = 1;
    if (dx < 0)
    {
        dirx = -1;
        dx = -dx;
    }
    if (dy < 0)
    {
        diry = -1;
        dy = -dy;
    }

    int fps = 1;  // TODO use width
    int fire = 0; // When this reaches fps a drop is fired

    long i;
    long over=0;
    if (dx > dy)
    {
        for (i = 0; i < dx; ++i)
        {
            x_axis.move_incremental(dirx);
            x_axis.wait_for_move();
            over += dy;
            if (over >= dx)
            {
                over -= dx;
                y_axis.move_incremental(diry);
                y_axis.wait_for_move();
            }

            fire++;
            if (fire >= fps)
            {
                fire_head(2,2,0,0);
                delay(10);
                fire = 0;
            }
        }
    }
    else
    {
        for (i = 0; i < dy; ++i)
        {
            y_axis.move_incremental(diry);
            y_axis.wait_for_move();
            over += dx;
            if (over >= dy)
            {
                over -= dy;
                x_axis.move_incremental(dirx);
                x_axis.wait_for_move();
            }

            fire++;
            if (fire >= fps)
            {
                fire_head(2,2,0,0);
                delay(10);
                fire = 0;
            }
        }
    }

    xpos = x;
    ypos = y;
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

    //Motor *motor = NULL;
    Stepper *motor = NULL;

    if(toupper(axis) == 'X') {
        //motor = xMotor;
        motor = x_axis.get_motor();
    } else if(toupper(axis) == 'Y') {
        //motor = yMotor;
        motor = y_axis.get_motor();
    } else {
        logger.error("No axis");
        return;
    }

    if(power == '0') {
        //motor->power(0);
        motor->enable(false);
    } else if (power == '1') {
        //motor->power(1);
        motor->enable(true);
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

char hexdig(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return 0;
}

void fire_spec(char *spec)
{
    byte a, f1, f2;

    a = hexdig(spec[0]);
    f1 = (hexdig(spec[1]) << 4) | hexdig(spec[2]);
    f2 = (hexdig(spec[3]) << 4) | hexdig(spec[4]);
    fire_head(f1, a, f2, a);
}

void fire_command(void) {
    char *spec = serial_command.next();
    fire_spec(spec);
}

void draw_command(void) {
    char *spec = serial_command.next();

    if(spec == NULL) {
        logger.error("Missing firing parameter");
        return;
    }

    char *srate = serial_command.next();
    if(srate == NULL) {
        logger.error("Missing firing rate");
        return;
    }

    char *arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing axis parameter");
        return;
    }

    char axis_id = arg[0];

    arg = serial_command.next();

    if(arg == NULL) {
        logger.error("Missing steps parameter");
        return;
    }

    long steps = atol(arg);
    float rate = atof(srate);

    Axis *axis = axis_from_id(axis_id);
    if (axis == NULL) {
        logger.error("Bad axis");
        return;
    }

    axis->move_incremental(steps);

    if (axis_id == 'X') {
        xpos += steps;
    } else {
        ypos += steps;
    }

    logger.info() << "Firing " << spec << " at rate " << rate << Comms::endl;

    float acc = 0;
    while(axis->moving()) {
        if (axis->run()) {
            acc += rate;
            while (acc >= 1.0) {
                fire_spec(spec);
                acc -= 1.0;
            }
        }
    }
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
            //logger.info("Something went wrong in readFile, aborting print.");
            //return;
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

void ls_command(void) {
    SdFile file;
    char name[256];

    sd.vwd()->rewind();

    int count = 0;
    while (file.openNext(sd.vwd(), O_READ)) {
        file.getLongFilename(name);

        if(strstr(name, ".HEX") || strstr(name, ".hex")) {
            logger.info(name);
            count++;
        }

        file.close();
    }

    if (count == 0)
        logger.info("No files.");
}

void md5_command(void) {
    char *arg = serial_command.next();

    SdFile file;
    file.open(arg);

    if (!file.isOpen()) {
        Serial.print("File could not be opened: ");
        Serial.println(arg);

        return;
    }

    byte block[1024];
    MD5_CTX md5;
    MD5_Init(&md5);
    for (;;)
    {
        int n = file.read(block, sizeof(block));
        if (n <= 0)
            break;
        MD5_Update(&md5, block, (unsigned int)n);
    }
    file.close();

    MD5_Final(block, &md5);
    int i;
    for (i = 0; i < 16; i++)
    {
        char lo = block[i] & 0xf;
        char hi = (block[i] >> 4) & 0xf;
        block[16 + i*2]     = hi >= 10 ? hi + 'a' - 10 : hi + '0';
        block[16 + i*2 + 1] = lo >= 10 ? lo + 'a' - 10 : lo + '0';
    }
    block[16+16*2] = 0;

    Serial.println((char*)block + 16);
}

void djb2_command(void) {
    char *arg = serial_command.next();

    SdFile file;
    file.open(arg);

    if (!file.isOpen()) {
        Serial.print("File could not be opened: ");
        Serial.println(arg);

        return;
    }

    byte block[1024];
    uint32_t hash = 5381;
    bool bFirst = true;

    for (;;)
    {
        int len = file.read(block, sizeof(block));
        if (len <= 0)
            break;

        if (bFirst && len >= 10 && block[0] == '#' && block[1] == ' ' &&
                block[10] == '\n')
        {
            // easy out
            block[10] = 0;
            Serial.println((char*)block + 2);
            file.close();
            return;
        }

        bFirst = false;

        int pos;
        for (pos = 0; pos < len; pos++)
        {
            int c = block[pos];
            hash = ((hash << 5) + hash) + c;
        }
    }

    file.close();

    int i;
    for (i = 0; i < 8; i++)
    {
        uint8_t v = (hash >> (28 - i*4)) & 0xf;
        block[i] = v >= 10 ? v + 'a' - 10 : v + '0';
    }
    block[8] = 0;
    Serial.println((char*)block);
}

void recv_command(void) {
    char *arg = serial_command.next();
    uint32_t size = 0;
    while (*arg >= '0' && *arg <= '9')
        size = size * 10 + (*arg++ - '0');

    char *filename = serial_command.next();
    bool compressed = false;
    if (!strcmp(filename, "b"))
    {
        compressed = true;
        filename = serial_command.next();
        decb_init();
    }
    SdFile file;
    file.open(filename, O_CREAT|O_WRITE|O_TRUNC);
    if (!file.isOpen()) {
        Serial.print("File could not be opened: ");
        Serial.println(filename);
        return;
    }

    Serial.println("Ready");

#define OVERLAP 64
    byte block[1028 + OVERLAP];
    byte block2[512 + OVERLAP];
    uint32_t hash = 5381;
    uint32_t pos = 0;
    int inoff = 0;
    while (pos < size)
    {
        if (compressed && inoff > OVERLAP)
        {
            // eep
            Serial.write((byte*)"J", 1);
            return;
        }
        uint32_t nleft = size - pos;
        int blocksize = nleft < 1024 ? nleft : 1024;
        int nread = blocksize + 4;
        int where = inoff;
        int len;
        while (nread > 0)
        {
            len = Serial.readBytes((char*)block + where, nread);
            if (len <= 0)
                break;
            if (len == 1 && where == inoff && *block == 'C')
            {
                file.remove();
                return;
            }
            nread -= len;
            where += len;
        }
        if (nread != 0)
        {
            Serial.println("Errorecv");
            Serial.println(nread);
            Serial.println(blocksize);
            Serial.println(pos);
            Serial.println(size);
            return;
        }

        uint32_t oldhash = hash;
        int n;
        for (n = 0; n < blocksize; n++)
        {
            block[inoff + n] ^= 0x26;
            int c = block[inoff + n];
            hash = ((hash << 5) + hash) + c;
        }
        byte bhash[4];
        bhash[0] = ( hash        & 0xff);
        bhash[1] = ((hash >>  8) & 0xff);
        bhash[2] = ((hash >> 16) & 0xff);
        bhash[3] = ((hash >> 24) & 0xff);
        if (memcmp(bhash, &block[inoff + blocksize], 4) != 0)
        {
            Serial.write((byte*)"B", 1);
            hash = oldhash;
            continue;
        }

        pos += blocksize;
        len = inoff + blocksize;
        inoff = 0;

        if (compressed)
        {
            int res = KEEP_GOING;
            while (res == KEEP_GOING)
            {
                int outlen = sizeof(block2);
                res = decb((char*)block, &inoff, len, (char*)block2, &outlen);
                if (res == DECODE_ERROR)
                {
                    // TODO: see if we can report bad block and unwind
                    Serial.write((byte*)"F", 1);
                    return;
                }
                file.write(block2, outlen);
            }

            memmove(block, block+inoff, len-inoff);
            inoff = len-inoff;
        }
        else
        {
            file.write(block, blocksize);
        }

        Serial.write((byte*)"G", 1);
    }

    file.close();
}


void help_command(void) {
    comms.println("Press p to print output.hex");
    comms.println("S to stop, P to pause, R to resume, c to calibrate.");
    comms.println("Additional commands: ");
    serial_command.installed_commands();
    comms.println();
}

#include "version.h"

void version_command(void) {
    comms.println(version_string);
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
    if(!sd.begin(53, SPI_HALF_SPEED)) {
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
/* Command set:
    + move up by some small amount.
    - move down by some small amount.
    E enable
    e disable
    r retract
    d deploy
    g get current servo angle.
    R Set retract position to current possition. Do not move.
    D Set Deploy position to current position, do not move.
    n where n is an integer number to set the servo angle to.
*/

  char *arg;
  int aval;	// angle value of servo, assum at 90 deg. to start with.
  aval=rollers.getangle();

  arg = serial_command.next();
/*
  logger.info("Got the roller command: ");
  logger.info(arg);
  logger.info("Current angle is:");
  logger.info(aval);
*/

  if(!arg) {
    logger.error("Missing position argument");
    logger.info("Valid options are:\n+ up\n - down\nE enable\ne disable\nr retract\nd deploy\nR Set retract pos.\nD Set deploy pos.\nangle, an integer\n");
    return;
  }


  switch(*arg)
  {
    case '+':
      logger.info("Raising the rollers");
      rollers.angle(--aval); // Smaller angles rotate up.
      break;
    case '-':
      logger.info("lowering the rollers");
      rollers.angle(++aval); // Larger angles rotate down.
      break;
    case 'E':
      logger.info("Enabling the rollers");
      rollers.enable();
      break;
    case 'e':
      logger.info("Disabling the rollers");
      rollers.disable();
      break;
    case 'r':
      logger.info("Retracting the rollers");
      rollers.retract();
      break;
    case 'd':
      logger.info("Deploying the rollers");
      rollers.deploy();
      break;
    case 'g':
      logger.info("The current server angle is:");
      logger.info(rollers.getangle());
      break;
    case 'R':
      logger.info("Setting the roller retract position");
      rollers.setrp(aval);
      break;
    case 'D':
      logger.info("Setting the roller deploy position");
      rollers.setdp(aval);
      break;
    default:
      int value = (unsigned char)atoi(arg);
      if(value > 0 && value <= 180) // Do not include zero, as a non-number apparently converts to zero, causing bad behavior.
      {															// Typical usefull values are from 50 to 90, outside that is unlikely if assembled as per the docs.
        logger.info("Setting the angle of the rollers to:");
        logger.info(value);
        rollers.angle(value);
      }
      else
        logger.info("Valid options are:\n+ up\n - down\nE enable\ne disable\nr retract\nd deploy\nR Set retract pos.\nD Set deploy pos.\nangle, an integer\n");
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

void axis_pos(void) {
    logger.info() << "X: " << x_axis.get_current_position() << " mm, "
            << "Y: " << y_axis.get_current_position() << " mm"
            << Comms::endl;
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

    logger.info() << "Primitive Voltage: " << voltage << " volts."
        << Comms::endl;
}
