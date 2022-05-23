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
	
	axis0 = joystick.get_axis(0) #horizontal
	axis1 = joystick.get_axis(1) #vertical
	
	
	if (abs(axis0) >= 0.01 and abs(axis1) >= 0.01): # Only act when both axes are far enough from 0 (helps calibrate and avoids divide by 0)
		angle = (180/math.pi)*math.atan(axis1/axis0)
		if (axis0 < 0):
			angle = (angle + 180) % 360
		turtle.setheading(angle)
		turtle.forward(int(5 * math.sqrt(math.pow(axis0,2) + math.pow(axis1,2)))))
	
	clock.tick(60)

pygame.quit()