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

#include "logging.h"

#include <Arduino.h>

Logger logger;
LoggerWrapper wrapper_info(&logger, Logger::Info);
LoggerWrapper wrapper_warn(&logger, Logger::Warn);
LoggerWrapper wrapper_error(&logger, Logger::Error);

const char *Logger::level_prefixes[] = {
    "+",
    "/",
    "-"
};

Logger::Logger() {
    minimum_log_level = Logger::Info;
    enabled = false;
}

Logger::~Logger() {

}

LoggerWrapper & Logger::info(void) {
    emit_prefix_for_level(Logger::Info);

    return wrapper_info;
}

LoggerWrapper & Logger::warn(void) {
    emit_prefix_for_level(Logger::Warn);

    return wrapper_warn;
}

LoggerWrapper & Logger::error(void) {
    emit_prefix_for_level(Logger::Error);

    return wrapper_error;
}

void Logger::emit_prefix_for_level(uint8_t level) {
    //log_for_level(millis(), level);
    //log_for_level(" - [", level);
    log_for_level(level_prefixes[level], level);
    //log_for_level("] ", level);
}

LoggerWrapper::LoggerWrapper(Logger *logger, uint8_t log_level) {
    this->logger = logger;
    this->log_level = log_level;
}

LoggerWrapper::~LoggerWrapper() {

}
