#include <Servo.h>

#define LEFT_THRUSTER_PIN 11
#define VERTICAL_THRUSTER_PIN 10
#define RIGHT_THRUSTER_PIN 9

#define HORIZONTAL_CLAW_PIN_1 8
#define HORIZONTAL_CLAW_PIN_2 7
#define VERTICAL_CLAW_PIN_1 6
#define VERTICAL_CLAW_PIN_2 5

#define GROUT_CARRIER_PIN 4
#define FISH_CARRIER_PIN 3

#define EXPECTED_VALUES 7



// This program expects the serial packets to have the following values in the order they are declared:
short lServoValue;
short verticalServoValue;
short rServoValue;

int horizontalClawValue;
int verticalClawValue;

byte groutCarrierValue;
byte fishCarrierValue;
// End of expected values

String packetString;

int packetScanPosition;
String currentPacketScanValue;

String packets[] = {
  "[1700, 1500, 1700, 0, 0, 0, 0]\n",
  "[1300, 1500, 1300, 0, 0, 0, 0]\n",
  "[1900, 1500, 1500, 0, 0, 0, 0]\n",
  "[1100, 1500, 1500, 0, 0, 0, 0]\n",
  "[1300, 1500, 1700, 0, 0, 0, 0]\n",
  "[1700, 1500, 1300, 0, 0, 0, 0]\n",
  "[1500, 1500, 1900, 0, 0, 0, 0]\n",
  "[1500, 1500, 1100, 0, 0, 0, 0]\n"
};

Servo thruster1;
Servo thruster2;
Servo thruster3;

int j;

void setup()
{
  thruster1.attach(LEFT_THRUSTER_PIN);
  thruster2.attach(VERTICAL_THRUSTER_PIN);
  thruster3.attach(RIGHT_THRUSTER_PIN);
  
  // Send the stop signals to the ESCs
  thruster1.writeMicroseconds(1500);
  thruster2.writeMicroseconds(1500);
  thruster3.writeMicroseconds(1500);
  
  
  pinMode(13, OUTPUT);
  
  pinMode(HORIZONTAL_CLAW_PIN_1, OUTPUT);
  pinMode(HORIZONTAL_CLAW_PIN_2, OUTPUT);
  pinMode(VERTICAL_CLAW_PIN_1, OUTPUT);
  pinMode(VERTICAL_CLAW_PIN_2, OUTPUT);
  
  pinMode(GROUT_CARRIER_PIN, OUTPUT);
  pinMode(FISH_CARRIER_PIN, OUTPUT);
  
  
  Serial.begin(9600);
  
  Serial.println("================================");
  Serial.println("ARDUINO CONTROL PROGRAM STARTED.");
  Serial.println("================================\n");
  
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
  j = 0;
  while (j < sizeof(packets))
  {
    packetString = packets[j];
    for(int i = 0; i < 50; i++)
    {
      digitalWrite(13, HIGH);
      delay(50);
      digitalWrite(13, LOW);
      delay(50);
    }
    thruster1.writeMicroseconds(1500);
    thruster2.writeMicroseconds(1500);
    thruster3.writeMicroseconds(1500);
    for(int i = 0; i < 50; i++)
    {
      digitalWrite(13, HIGH);
      delay(50);
      digitalWrite(13, LOW);
      delay(50);
    }
    
    // Strip any characters before the '['
    packetString.remove(0,packetString.indexOf('['));
    
    // Strip any characters after the ']' (will result in an empty string if there is no ']')
    packetString.remove(packetString.indexOf(']')+1);
    
    
    packetScanPosition = 0;
    for (int i = 0; i < EXPECTED_VALUES; i++)
    {
      while ((packetScanPosition + 1) < packetString.length())
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
        case 0: // Left thruster
          if(currentPacketScanValue.toInt() == 0)
            currentPacketScanValue = 1500;
          lServoValue = currentPacketScanValue.toInt();
          break;
        case 1: // Vertical thruster
          if(currentPacketScanValue.toInt() == 0)
            currentPacketScanValue = 1500;
          verticalServoValue = currentPacketScanValue.toInt();
          break;
        case 2: // Right thruster
          if(currentPacketScanValue.toInt() == 0)
            currentPacketScanValue = 1500;
          rServoValue = currentPacketScanValue.toInt();
          break;
        case 3: // Horizontal claw
          horizontalClawValue = currentPacketScanValue.toInt();
          break;
        case 4: // Vertical claw
          verticalClawValue = currentPacketScanValue.toInt();
          break;
        case 5: // Grout carrier
          groutCarrierValue = currentPacketScanValue.toInt();
          break;
        case 6: // Fish carrier
          fishCarrierValue = currentPacketScanValue.toInt();
          break;
      }
      
      currentPacketScanValue = "";
    }
    
    
    Serial.println("=================");
    Serial.println("packetString: " + packetString);
    Serial.println("lServoValue: " + String(lServoValue));
    Serial.println("verticalServoValue: " + String(verticalServoValue));
    Serial.println("rServoValue: " + String(rServoValue));
    Serial.println("horizontalClawValue: " + String(horizontalClawValue));
    Serial.println("verticalClawValue: " + String(verticalClawValue));
    Serial.println("groutCarrierValue: " + String(groutCarrierValue));
    Serial.println("fishCarrierValue: " + String(fishCarrierValue));
    Serial.println("=================\n\n\n");
    Serial.flush();
    
    
    thruster1.writeMicroseconds(limitMS(lServoValue));
    thruster2.writeMicroseconds(limitMS(verticalServoValue));
    thruster3.writeMicroseconds(limitMS(rServoValue));
    
    bidirectionalWrite(HORIZONTAL_CLAW_PIN_1, HORIZONTAL_CLAW_PIN_2, horizontalClawValue);
    bidirectionalWrite(VERTICAL_CLAW_PIN_1, VERTICAL_CLAW_PIN_2, verticalClawValue);
    
    unidirectionalWrite(GROUT_CARRIER_PIN, groutCarrierValue);
    unidirectionalWrite(FISH_CARRIER_PIN, fishCarrierValue);


    j++;
  }
}


short limitMS (int servoMicroseconds)
{
  // Limits the microsecond values between 1100 and 1900
  if (servoMicroseconds > 1900)
    return 1900;
  else if (servoMicroseconds < 1100)
    return 1100;
  else
    return (short)servoMicroseconds;
}

void bidirectionalWrite (byte pin1, byte pin2, int value)
{
  switch (value)
  {
    case -1:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      break;
    case 0:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;
    case 1:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      break;
    default:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;
  }
}

void unidirectionalWrite (byte pin, int value)
{
  switch (value)
  {
    case 0:
      digitalWrite(pin, LOW);
      break;
    case 1:
      digitalWrite(pin, HIGH);
      break;
    default:
      digitalWrite(pin, LOW);
      break;
  }
}
