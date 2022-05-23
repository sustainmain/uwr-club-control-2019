#include <Servo.h>

#define THRUSTER_1_PIN 11
#define THRUSTER_2_PIN 10
#define THRUSTER_3_PIN 9

#define RELATIVE_SPEED 5

#define FLOAT_DECIMAL_PLACES 3

#define EXPECTED_VALUES 4


// This program expects the serial packets to have the following values in the order they are declared:
float joystick1Power;
short joystick1Angle;

float joystick2Power;
short joystick2Angle;

float joystick3Power;
short joystick3Angle;
// End of expected values

char incomingChar;
String packetString;

int packetScanPosition;
String currentPacketScanValue;

Servo thruster1;
Servo thruster2;
Servo thruster3;

char floatBuffer[32];



void setup()
{
  Serial.begin(9600);
  
  
  thruster1.attach(THRUSTER_1_PIN);
  thruster2.attach(THRUSTER_2_PIN);
  thruster3.attach(THRUSTER_3_PIN);
  
  // Send the stop signals to the ESCs
  thruster1.writeMicroseconds(1500);
  thruster2.writeMicroseconds(1500);
  thruster3.writeMicroseconds(1500);

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
            joystick1Power = atof(floatBuffer);
            break;
          case 1:
            joystick1Angle = currentPacketScanValue.toInt();
            break;
          case 2:
            currentPacketScanValue.toCharArray(floatBuffer, sizeof(floatBuffer));
            joystick2Power = atof(floatBuffer);
            break;
          case 3:
            joystick2Angle = currentPacketScanValue.toInt();
            break;
        }
        
        currentPacketScanValue = "";
      }
      
      packetString = "";
      
      /*
      Serial.println("=================");
      Serial.println("joystick1Power: " + String(joystick1Power));
      Serial.println("joystick1Angle: " + String(joystick1Angle));
      Serial.println("joystick2Power: " + String(joystick2Power));
      Serial.println("joystick2Angle: " + String(joystick2Angle));
      Serial.println("joystick3Power: " + String(joystick3Power));
      Serial.println("joystick3Angle: " + String(joystick3Angle));
      Serial.println("=================\n\n\n");
      */
    }
    else
    {
      packetString += incomingChar;
    }
  }
  
  thruster1.writeMicroseconds(calculateThruster1(joystick1Power, joystick1Angle));
  thruster2.writeMicroseconds(calculateThruster2(joystick1Power, joystick1Angle));
  thruster3.writeMicroseconds(calculateThruster3(joystick2Power, joystick2Angle));
  
}


int calculateThruster1(float power, short angle)
{
  return (1500 + (400*(power/RELATIVE_SPEED)*cos(radians(angle))));
}

int calculateThruster2(float power, short angle)
{
  return (1500 + (-400*(power/RELATIVE_SPEED)*cos(radians(angle))));
}

int calculateThruster3(float power, short angle)
{
  return (1500 + (400*(power/RELATIVE_SPEED)*sin(radians(angle))));
}
