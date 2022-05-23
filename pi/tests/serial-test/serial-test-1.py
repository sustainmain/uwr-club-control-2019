import RPi.GPIO as GPIO
import time

ledPin = 11

GPIO.setmode(GPIO.BOARD)
GPIO.setup(ledPin, GPIO.OUT)

while True:
    print("led on")
    GPIO.output(ledPin, GPIO.HIGH)
    time.sleep(1.0)
    print("led off")
    GPIO.output(ledPin, GPIO.LOW)
    time.sleep(1.0)
