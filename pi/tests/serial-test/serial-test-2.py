import time
import serial

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)

i = 0

while True:
    print(i)
    ser.write(bytes(str(i)+"\n", 'utf-8'))
    time.sleep(1)
    i += 1
