#!/usr/bin/env python3
import serial
import time
if __name__ == '__main__':

    connected = False
    ser = None
    port = 'com3'
    for i in range(0, 20):
        try:
            port = 'com' + str(i)
            ser = serial.Serial(port, 9600, timeout=1)  # ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
            ser.reset_input_buffer()
        except:
            continue
        else:
            print("Connected to port", port)
            connected = True
            break

    if not connected:
        print("Could not connect serial")

    while True:
        ser.write(b"Hello from Raspberry Pi!\n")
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(1)
