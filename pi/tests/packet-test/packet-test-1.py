import time
import serial
import sys

sampleDelay = 3

packet_strings = [
    "[1500, 1500, 1500]\n", # Normal
    "[15[1500, 1500, 1500]\n", # Dropped end
    "[[150\n",
    "\n",
    "[[[]]]\n",
    "]\n",
    "[\n",
    "[[[[\n",
    "][\n",
    "][[[\n",
    "]]][\n",
    "]]][[[\n"
]

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = sampleDelay
)

print()
print("Started Packet Test.")
print()
sys.stdout.flush()


while True:
    for packet_string in packet_strings:
        ser.write(bytes(packet_string, 'utf-8'))
        sys.stdout.write(packet_string)
        sys.stdout.flush()
        
        time.sleep(sampleDelay)
