#ifndef _COMMS_H_
#define _COMMS_H_

#include <stdint.h>
#include <Arduino.h>

namespace Comms {
    static const char *endl = "\r\n";
}

class SerialChannel {
public:
    SerialChannel(uint32_t baudrate);
    ~SerialChannel();

    void initialise(void);

    void write(const void *data, uint8_t length);

    template<class T> void send(const T arg) {
        Serial.print(arg);
    }

    /*template<class T> void println(const T arg) {
        send(arg);
        send(Comms::endl);
    }

    void println(void);*/

private:
    uint32_t baudrate;
};

extern SerialChannel comms;

#endif
