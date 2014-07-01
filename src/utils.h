#ifndef _UTILS_H_
#define _UTILS_H_

#include "settings.h"

int ram_free(void);
double ram_utilisation(void);
void print_settings(AxisSettings *settings);

#endif
