#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdint.h>
#include <Arduino.h>

// Because the definition comes after
class LoggerWrapper;

class Logger {
public:
    enum LogLevels {
        Info  = 0,
        Warn  = 1,
        Error = 2
    };

    static const char *endl;

    static const char *level_prefixes[];

    Logger();
    ~Logger();

    LoggerWrapper & info(void);

    template<class T> inline LoggerWrapper & info(T arg) {
        return info() << arg << Logger::endl;
    }

    LoggerWrapper & warn(void);

    template<class T> inline LoggerWrapper & warn(T arg) {
        return warn() << arg << Logger::endl;
    }

    LoggerWrapper & error(void);

    template<class T> inline LoggerWrapper & error(T arg) {
        return error() << arg << Logger::endl;
    }

    /*
    template<class T> inline Logger &operator <<(T arg) {
        Serial.print(arg);

        return *this;
    }
    */

    uint8_t minimum_log_level;
    bool enabled;

private:

    template<class T> void log_for_level(T entry, uint8_t level) {
        /*Serial.print(level);
        Serial.print(" for ");
        Serial.println(entry);*/

        if(enabled && level >= minimum_log_level) {
            Serial.print(entry);
        }
    }

    void prefix_for_level(uint8_t level);

    friend class LoggerWrapper;
};

class LoggerWrapper {
public:
    LoggerWrapper(Logger *logger, uint8_t log_level);
    ~LoggerWrapper();

    template<class T> inline LoggerWrapper &operator <<(T arg) {
        _logger->log_for_level(arg, _log_level);

        return *this;
    }

private:
    Logger *_logger;
    uint8_t _log_level;
};

extern Logger logger;

#endif
