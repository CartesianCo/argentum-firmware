#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

#define RAM_TOTAL 8192

int ram_free(void);
int ram_used(void);
double ram_utilisation(void);
void print_settings(AxisSettings *settings);

#endif
