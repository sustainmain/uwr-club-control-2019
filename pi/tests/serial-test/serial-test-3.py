import time
import serial
import sys

#sampleDelay = 1

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 0.1
)

while True:
    line = ser.readline().decode('utf-8')

    #print(line)
    sys.stdout.write(line)
