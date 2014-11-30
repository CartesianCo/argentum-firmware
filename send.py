#!/usr/bin/python

import sys
import os
from serial.tools.list_ports import comports
from serial import Serial

def findPort():
    for port in comports():
        if (port[2].find("2341:0042") != -1 or
            port[2].find("2341:42") != -1):
            return port[0]
    return None

def waitForResponse(serialDevice, timeout=0.5, expect=None):
    serialDevice.timeout = timeout
    response = ""
    try:
        while True:
            data = serialDevice.read(1)
            n = serialDevice.inWaiting()
            if n > 0:
                #print("waitForResponse reading {} more bytes.".format(n))
                data = data + serialDevice.read(n)
            else:
                break
            if data:
                response = response + data.decode('utf-8', 'ignore')

            if expect:
                if response.find(expect) != -1:
                    break
    finally:
        serialDevice.timeout = 0

    return response

def main(args):
    if len(args) < 1:
        print("usage: send <cmd>")
        sys.exit(1)
    cmd = ' '.join(args)

    port = findPort()
    if port == None:
        print("can't find the printer.")
        sys.exit(1)

    serialDevice = Serial(port, 115200, timeout=0)
    response = waitForResponse(serialDevice, 3.0, '\n')
    if response.find('\n') == -1 or response.find('.') == -1:
        print("printer did not respond.")
        sys.exit(1)

    serialDevice.write(cmd + "\n")

    try:
        while True:
            data = serialDevice.read(1)
            if data:
                sys.stdout.write(data.decode('utf-8', 'ignore'))
    except:
        pass

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit(0)
