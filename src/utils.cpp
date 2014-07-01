#include "utils.h"

#include "settings.h"

int ram_used(void) {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int ram_free(void) {
    return RAM_TOTAL - ram_used();
}

double ram_utilisation(void) {
    return (ram_used() / (RAM_TOTAL / 100.0)); // 8192 bytes total * 100 to be percent.
}

void print_settings(AxisSettings *settings) {
    Serial.println("Axis Settings:");
    Serial.print(settings->axis);
    Serial.print(" -> ");
    Serial.print(settings->motor);

    if(settings->flipped) {
        Serial.print(" Flipped, ");
    } else {
        Serial.print(" Not Flipped, ");
    }

    Serial.print(settings->length);
    Serial.println(" steps");
}
