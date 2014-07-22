#include "comms.h"

SerialChannel comms(115200);

SerialChannel::SerialChannel(uint32_t baudrate) {
    this->baudrate = baudrate;
}

SerialChannel::~SerialChannel() {
    Serial.end();
}

void SerialChannel::initialise(void) {
    Serial.begin(baudrate);
}

void SerialChannel::write(const void *data, uint8_t length) {
    Serial.write((const uint8_t *)data, length);
}

void SerialChannel::println(void) {
    Serial.println();
}
