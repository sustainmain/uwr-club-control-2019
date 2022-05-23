#!/usr/bin/env python

import RPi.GPIO as GPIO
import subprocess
import time
import sys


shutdown_button_pin = 16
reset_button_pin = 18

button_hold = 0.5 # Number of seconds a button must be held down to execute the button's action

print('\n\n\n\nbutton-listener started.\n\n')
sys.stdout.flush()

def shutdown_button_callback(channel):
    if (GPIO.input(channel) == GPIO.LOW):
        print('Shutdown button pressed.')

        button_released = False
        start_time = time.time()
        while (button_hold > time.time() - start_time):
            if (GPIO.input(channel) == GPIO.HIGH):
                button_released = True
                break

        if (not button_released):
            print('Shutdown button was held down long enough.')
            print('Stopping pi-control...')
            sys.stdout.flush()
            subprocess.call(['service', 'pi-control', 'stop'], shell=False)
            print('pi-control stopped.')
            sys.stdout.flush()
            time.sleep(1)

            print('Shutting down...\n')
            sys.stdout.flush()
            subprocess.call(['shutdown', '-h', 'now'], shell=False)
        else:
            print('Shutdown button was not held down long enough.\n')
            sys.stdout.flush()

def reset_button_callback(channel):
    if (GPIO.input(channel) == GPIO.LOW):
        print('Reset button pressed.')

        button_released = False
        start_time = time.time()
        while (button_hold > time.time() - start_time):
            if (GPIO.input(channel) == GPIO.HIGH):
                button_released = True
                break

        if (not button_released):
            print('Reset button was held down long enough.')
            print('Restarting pi-control...')
            sys.stdout.flush()
            subprocess.call(['service', 'pi-control', 'restart'], shell=False)
            print('pi-control restarted.\n')
            sys.stdout.flush()
        else:
            print('Reset button was not held down long enough.\n')
            sys.stdout.flush()
    else:
        print('Reset button released.\n')
        sys.stdout.flush()


GPIO.setmode(GPIO.BOARD)
GPIO.setup(shutdown_button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(shutdown_button_pin, GPIO.BOTH, callback=shutdown_button_callback, bouncetime=300)

GPIO.setup(reset_button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(reset_button_pin, GPIO.BOTH, callback=reset_button_callback, bouncetime=300)

while True:
    time.sleep(10)
