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

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdint.h>
#include <Arduino.h>

#include "comms.h"

// Because the definition comes after
class LoggerWrapper;

class Logger {
public:
    enum LogLevels {
        Info  = 0,
        Warn  = 1,
        Error = 2
    };

    static const char *level_prefixes[];

    Logger();
    ~Logger();

    LoggerWrapper & info(void);

    template<class T> inline LoggerWrapper & info(T arg) {
        return info() << arg << Comms::endl;
    }

    LoggerWrapper & warn(void);

    template<class T> inline LoggerWrapper & warn(T arg) {
        return warn() << arg << Comms::endl;
    }

    LoggerWrapper & error(void);

    template<class T> inline LoggerWrapper & error(T arg) {
        return error() << arg << Comms::endl;
    }

    uint8_t minimum_log_level;
    bool enabled;

private:
    template<class T> void log_for_level(T entry, uint8_t level) {
        if(enabled && level >= minimum_log_level) {
            comms.send(entry);
        }
    }

    void emit_prefix_for_level(uint8_t level);

    friend class LoggerWrapper;
};

class LoggerWrapper {
public:
    LoggerWrapper(Logger *logger, uint8_t log_level);
    ~LoggerWrapper();

    template<class T> inline LoggerWrapper &operator <<(T arg) {
        logger->log_for_level(arg, log_level);

        return *this;
    }

private:
    Logger *logger;
    uint8_t log_level;
};

extern Logger logger;

#endif
