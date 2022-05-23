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
    output = []
    for event in pygame.event.get():
        #print(event)
        if (event.type == pygame.QUIT):
            done = True

    # If both axes are below their thresholds, set both axes to 0 (this helps with calibration)
    if ((abs(x) <= xThreshold) and (abs(y) <= yThreshold)):
        x = 0
        y = 0

    distance = (relativeSpeed * math.sqrt(x**2 + y**2))
    angle = (180/math.pi) * math.atan2(y,x)
    
    if (angle < 0):
        angle += 360
    
    if (distance > relativeSpeed):
        distance = relativeSpeed
    
    output.append(round(distance, 3))
    output.append(int(angle))
    return output


while True:
    x_axis = joystick.get_axis(0)        # Horizontal
    y_axis = -1 * joystick.get_axis(1)   # Vertical
    
    joystick_string = str(cartesianToPolar(x_axis, y_axis)) + "\n"
    ser.write(bytes(joystick_string, 'utf-8'))
    print(joystick_string)
    #sys.stdout.write(joystick_string)
    
    time.sleep(0.1)
