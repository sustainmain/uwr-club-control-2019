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
  // Ramp up to full power
  for(int i = 1500; i <= 1900; i++)
  {
    xThruster.writeMicroseconds(i);
    delay(10);
  }

  // Hold for 1 second
  delay(1000);
  
  // Ramp down to a stop
  for(int i = 1900; i >= 1500; i--)
  {
    xThruster.writeMicroseconds(i);
    delay(10);
  }
  
  // 20 second break
  delay(20000);
}
