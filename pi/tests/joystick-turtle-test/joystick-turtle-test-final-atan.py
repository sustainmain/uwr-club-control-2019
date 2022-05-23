import pygame
import turtle
import math

pygame.init()
clock = pygame.time.Clock()

t = turtle.Pen()

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


while (not done):
    for event in pygame.event.get():
        #print(event)
        if (event.type == pygame.QUIT):
            done = True
    
    x = joystick.get_axis(0)        # Horizontal
    y = -1 * joystick.get_axis(1)   # Vertical
    
    # If both axes are below their thresholds, set both axes to 0 (this helps with calibration)
    if ((abs(x) <= xThreshold) and (abs(y) <= yThreshold)):
        x = 0
        y = 0
    
    distance = int(relativeSpeed * math.sqrt(math.pow(x,2) + math.pow(y,2)))
    
    if (x == 0):    # Avoid dividing by 0; this code manually sets the angle based on where on the y-axis the joystick point is.
        if (y > 0):
            angle = 90
        elif (y < 0):
            angle = 270
    else:
        angle = (180/math.pi) * math.atan(y/x)      # math.atan only gives values in quadrants 1 and 4, which means that further processing is required
        
        if (x < 0):         # Point of joystick (x,y) must be in quadrants 2 or 3  
            angle += 180
        elif (y < 0):       # Point of joystick (x,y) must be in quadrant 4 (elif will not allow values in quadrant 3 to fall here)
            angle += 360    # This avoids negative angles

    if (distance != 0):
        turtle.setheading(angle)
        turtle.forward(distance)
    
    clock.tick(60)

pygame.quit()
