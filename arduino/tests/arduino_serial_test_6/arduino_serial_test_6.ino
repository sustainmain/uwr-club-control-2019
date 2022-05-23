#include <Servo.h>

#define THRUSTER_1_PIN 11

#define RELATIVE_SPEED 5

#define FLOAT_DECIMAL_PLACES 3

#define EXPECTED_VALUES 2


// This program expects the serial packets to have the following values in the order they are declared:
float thruster1Power;
short thruster1Angle;
// End of expected values

char incomingChar;
String packetString;

int packetScanPosition;
String currentPacketScanValue;

Servo thruster1;

char floatBuffer[32];



void setup()
{
  Serial.begin(9600);
  
  
  thruster1.attach(THRUSTER_1_PIN);
  
  thruster1.writeMicroseconds(1500); // Send the stop signal to the ESC

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
  if (Serial.available() > 0)
  {
    incomingChar = Serial.read();
    
    if (incomingChar == '\n')
    {
      // Strip any characters before the '['
      packetString.remove(0,packetString.indexOf('['));
      
      // Strip any characters after the ']' (will result in an empty string if there is no ']')
      packetString.remove(packetString.indexOf(']')+1);
      
      
      packetScanPosition = 0;
      for (int i = 0; i < EXPECTED_VALUES; i++)
      {
        while (packetScanPosition < (packetString.length() - 1))
        {
          if (packetString.charAt(packetScanPosition) == ',')
          {
            packetScanPosition++;
            break;
          }
          else if (packetString.charAt(packetScanPosition) != '[')
          {
            currentPacketScanValue += packetString.charAt(packetScanPosition);
          }
          
          packetScanPosition++;
        }
        
        
        switch (i)
        {
          case 0:
            currentPacketScanValue.toCharArray(floatBuffer, sizeof(floatBuffer));
            thruster1Power = atof(floatBuffer);
            break;
          case 1:
            thruster1Angle = currentPacketScanValue.toInt();
            break;
        }
        
        currentPacketScanValue = "";
      }
      
      packetString = "";
      
      /*
      Serial.println("=================");
      Serial.println("thruster1Power: " + String(thruster1Power));
      Serial.println("thruster1Angle: " + String(thruster1Angle));
      Serial.println(thruster1Power/RELATIVE_SPEED);
      Serial.println(cos(radians(thruster1Angle)));
      Serial.println(calculateServoMicroseconds(thruster1Power, thruster1Angle));
      Serial.println("=================\n\n\n");
      */
    }
    else
    {
      packetString += incomingChar;
    }
  }
  
  thruster1.writeMicroseconds(calculateServoMicroseconds(thruster1Power, thruster1Angle));
}


int calculateServoMicroseconds(float power, short angle)
{
  return (1500 + (400*(power/RELATIVE_SPEED)*cos(radians(angle))));
}
