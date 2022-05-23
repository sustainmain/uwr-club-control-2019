#include <Servo.h>

#define THRUSTER_1_PIN 11
#define THRUSTER_2_PIN 10
#define THRUSTER_3_PIN 9

#define EXPECTED_VALUES 3


// This program expects the serial packets to have the following values in the order they are declared:
short lServoValue;
short rServoValue;
short verticalServoValue;
// End of expected values

char incomingChar;
String packetString;

int packetScanPosition;
String currentPacketScanValue;

Servo thruster1;
Servo thruster2;
Servo thruster3;



void setup()
{
  pinMode(13, OUTPUT);
  
  thruster1.attach(THRUSTER_1_PIN);
  thruster2.attach(THRUSTER_2_PIN);
  thruster3.attach(THRUSTER_3_PIN);
  
  // Send the stop signals to the ESCs
  thruster1.writeMicroseconds(1500);
  thruster2.writeMicroseconds(1500);
  thruster3.writeMicroseconds(1500);
  
  Serial.begin(9600);
  
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
      // Strip any characters before the '[' (will result in an empty string if there is no '[')
      packetString.remove(0,packetString.indexOf('['));
      
      // Strip any characters after the ']' (will result in an empty string if there is no ']')
      packetString.remove(packetString.indexOf(']')+1);
      
      Serial.println("packetString: " + packetString);
      Serial.println("Packet length: " + String(packetString.length()));
      
      
      packetScanPosition = 0;
      for (int i = 0; i < EXPECTED_VALUES; i++)
      {
        Serial.println(packetScanPosition < (packetString.length() - 1));
        Serial.println(packetScanPosition);
        Serial.println(packetString.length() - 1); // This was intentionally left using the old version to demonstrate the problem
        while ((packetScanPosition + 1) < packetString.length()) // Used to be packetScanPosition < (packetString.length() - 1)
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
        
        Serial.println("currentPacketScanValue: " + String(currentPacketScanValue));
        Serial.println("casted currentPacketScanValue: " + String(currentPacketScanValue.toInt()));
        
        switch (i)
        {
          case 0:
            if(currentPacketScanValue.toInt() == 0)
              currentPacketScanValue = 1500;
            
            lServoValue = currentPacketScanValue.toInt();
            break;
          case 1:
            if(currentPacketScanValue.toInt() == 0)
              currentPacketScanValue = 1500;
            
            rServoValue = currentPacketScanValue.toInt();
            break;
          case 2:
            if(currentPacketScanValue.toInt() == 0)
              currentPacketScanValue = 1500;
            
            verticalServoValue = currentPacketScanValue.toInt();
            break;
        }
        currentPacketScanValue = "";
      }
      
      packetString = "";
      
      
      Serial.println("=================");
      Serial.println("lServoValue: " + String(lServoValue));
      Serial.println("rServoValue: " + String(rServoValue));
      Serial.println("verticalServoValue: " + String(verticalServoValue));
      Serial.println("=================\n\n\n");
      
    }
    else
    {
      packetString += incomingChar;
    }
  }
  /*
  thruster1.writeMicroseconds(lServoValue);
  thruster2.writeMicroseconds(rServoValue);
  thruster3.writeMicroseconds(verticalServoValue);
  */
}
