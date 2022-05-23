import pygame
import math
import time
import serial
#import sys

#sampleDelay = 1

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)

pygame.init()
clock = pygame.time.Clock()

done = False


xThreshold = 0.01
yThreshold = 0.01
relativeSpeed = 5


joystick = pygame.joystick.Joystick(0)
joystick.init()

print()
print("Joystick ID: " + str(joystick.get_id()))
print("Joystick Name: " + str(joystick.get_name()))
print("Initialized? " + str(joystick.get_init()))
print()



def cartesianToPolar (x,y):
    for event in pygame.event.get():
        #print(event)
        if (event.type == pygame.QUIT):
            done = True

    # If both axes are below their thresholds, set both axes to 0 (this helps with calibration)
    if ((abs(x) <= xThreshold) and (abs(y) <= yThreshold)):
        x = 0
        y = 0

    power = (relativeSpeed * math.sqrt(x**2 + y**2))
    angle = (180/math.pi) * math.atan2(y,x)
    
    if (angle < 0):
        angle += 360
    
    if (power > relativeSpeed):
        power = relativeSpeed
    
    return round(power, 3), int(angle)


def preparePacket ():
    packet = []
    
    x_axis_1 = joystick.get_axis(0)        # Left horizontal
    y_axis_1 = -1 * joystick.get_axis(1)   # Left vertical
    x_axis_2 = joystick.get_axis(2)        # Right horizontal
    y_axis_2 = -1 * joystick.get_axis(3)   # Right vertical
    
    packet.append(cartesianToPolar(x_axis_1, y_axis_1)[0])
    packet.append(cartesianToPolar(x_axis_1, y_axis_1)[1])
    packet.append(cartesianToPolar(x_axis_2, y_axis_2)[0])
    packet.append(cartesianToPolar(x_axis_2, y_axis_2)[1])
    
    return packet



while True:
    packet_string = str(preparePacket()) + "\n"
    ser.write(bytes(packet_string, 'utf-8'))
    print(packet_string)
    #sys.stdout.write(packet_string)
    
    time.sleep(0.1)
