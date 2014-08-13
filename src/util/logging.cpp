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
