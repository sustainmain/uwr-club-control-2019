import pygame
import math
import time
import serial
import sys

sampleDelay = 3

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = sampleDelay
)

pygame.init()
clock = pygame.time.Clock()

done = False


joystick = pygame.joystick.Joystick(0)
joystick.init()

print()
print("Joystick ID: " + str(joystick.get_id()))
print("Joystick Name: " + str(joystick.get_name()))
print("Initialized? " + str(joystick.get_init()))
print()
sys.stdout.flush()



def calculateServoValues (x,y):
    rServoTurn = x
    lServoTurn = -rServoTurn
    
    rServoForwardBackward = y
    lServoForwardBackward = rServoForwardBackward
    
    rServoRelative = (rServoTurn + rServoForwardBackward) / 2
    lServoRelative = (lServoTurn + lServoForwardBackward) / 2
    
    rServoValue = 1500 + (400 * rServoRelative)
    lServoValue = 1500 + (400 * lServoRelative)
    
    return rServoValue, lServoValue


def preparePacket ():
    packet = []
    
    x_axis_1 = joystick.get_axis(0)      # Left horizontal
    y_axis_1 = -1 * joystick.get_axis(1) # Left vertical
    x_axis_2 = joystick.get_axis(2)      # Right horizontal
    y_axis_2 = -1 * joystick.get_axis(3) # Right vertical
    
    horizontal_claw_open = joystick.get_button(4)
    horizontal_claw_close = joystick.get_button(6)
    if (horizontal_claw_open and not horizontal_claw_close):
        horizontal_claw_value = 1
    elif (horizontal_claw_close and not horizontal_claw_open):
        horizontal_claw_value = -1
    else:
        horizontal_claw_value = 0
    
    vertical_claw_open = joystick.get_button(5)
    vertical_claw_close = joystick.get_button(7)
    if (vertical_claw_open and not vertical_claw_close):
        vertical_claw_value = 1
    elif (vertical_claw_close and not vertical_claw_open):
        vertical_claw_value = -1
    else:
        vertical_claw_value = 0
    
    
    
    packet.append(round(calculateServoValues(x_axis_1, y_axis_1)[0]))
    packet.append(round(1500 + (400 * y_axis_2)))
    packet.append(round(calculateServoValues(x_axis_1, y_axis_1)[1]))
    packet.append(horizontal_claw_value)
    packet.append(vertical_claw_value)
    packet.append(int(joystick.get_button(0)))
    packet.append(int(joystick.get_button(1)))
    
    return packet



while (not done):
    for event in pygame.event.get():
        #print(event)
        if (event.type == pygame.QUIT):
            done = True
    
    packet_string = str(preparePacket()) + "\n"
    ser.write(bytes(packet_string, 'utf-8'))
    sys.stdout.write(packet_string)
    sys.stdout.flush()
    
    #time.sleep(sampleDelay)
    clock.tick(1/sampleDelay)
