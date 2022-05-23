#include <Servo.h>

const byte xThrusterPin = 11;
Servo xThruster;

void setup() {
  xThruster.attach(xThrusterPin);
  
  xThruster.writeMicroseconds(1500); // Send the stop signal to the ESC

  pinMode(13, OUTPUT);
  
  // Delay 5 seconds so the ESC can initialize
  for(int i = 0; i < 5; i++)
  {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
}

void loop()
{
  // Set the thruster to full power
  xThruster.writeMicroseconds(1900);
}
