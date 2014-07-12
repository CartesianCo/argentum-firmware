#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

#define RAM_TOTAL 8192

// This string prepending method to get the log level information is bad,
// inspecting the firmware file with strings shows that it is prepended to
// every string instance, taking up multiple times as much memory as it should.

// Propose moving this into a class. The overhead is negligible compared to the
// serial transmission anyway.
#define INFO
#define WARN
#define ERROR


#define INFO_PREFIX "INFO "
#define WARN_PREFIX "WARN "
#define ERROR_PREFIX "ERROR "

#define log(a)          Serial.print(a)

#ifdef INFO
    #define log_info(a)     log(INFO_PREFIX a)
    #define log_info_np(a)  log(a)
#else
    #define log_info(a)
    #define log_info_np(a)
#endif

#ifdef WARN
    #define log_warn(a)     log(WARN_PREFIX a)
    #define log_warn_np(a)  log(a)
#else
    #define log_warn(a)
    #define log_warn_np(a)
#endif

#ifdef ERROR
    #define log_error(a)        log(ERROR_PREFIX a)
    #define log_error_np(a) log(a)
#endif

int ram_free(void);
int ram_used(void);
double ram_utilisation(void);

uint8_t CRC8(const void *buffer, const uint16_t length);
uint8_t CRC8_add_byte(uint8_t data, uint8_t seed_crc);

#endif
