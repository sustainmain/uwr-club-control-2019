import pygame
import turtle
import math

pygame.init()
clock = pygame.time.Clock()

t = turtle.Pen()

done = False

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
		if event.type == pygame.QUIT:
			done = True
	
	axis0 = joystick.get_axis(0)       # Horizontal axis
	axis1 = -1 * joystick.get_axis(1)  # Vertical axis
	
	# If both axes are below their thresholds, set both axes to zero. This corrects miscalibrations.
	if ((abs(axis0) >= 0.01) and (abs(axis1) >= 0.01)):
		axis0 = 0
		axis1 = 0
	
	
	if (axis0 == 0):  # Avoid dividing by zero; this code manually sets the angle and distance based on where the point is rather than doing math
		distance = axis0
		if (axis1 > 0):
			angle = 90
		elif (axis1 < 0):
			angle = 270
	else:
		angle = (180/math.pi) * math.atan(axis1/axis0)
		if (axis0 < 0):
			angle = (angle + 180) % 360
			distance = int(5 * math.sqrt(math.pow(axis0,2) + math.pow(axis1,2)))
	
	
	
	if (distance != 0):  # Only try to act on the input if it registered above the threshold (distance would be 0 if the input was below threshold)
		turtle.setheading(angle)
		turtle.forward(distance)
	
	clock.tick(60)

pygame.quit()