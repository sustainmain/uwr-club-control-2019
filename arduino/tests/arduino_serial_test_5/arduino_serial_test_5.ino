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


char floatBuffer[32];


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    incomingChar = Serial.read();
    
    if (incomingChar == '\n')
    {
      Serial.println("Received string: " + packetString);
      
      // Strip any characters before the '['
      packetString.remove(0,packetString.indexOf('['));
      
      // Strip any characters after the ']' (will result in an empty string if there is no ']')
      packetString.remove(packetString.indexOf(']')+1);

      Serial.println("Processed string: " + packetString);
      Serial.println();
      
      packetScanPosition = 0;
      for (int i = 0; i < EXPECTED_VALUES; i++)
      {
        while (packetScanPosition < (packetString.length() - 1))
        {
          Serial.println("scanning " + String(packetString.charAt(packetScanPosition)));
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
        Serial.println();
        
        Serial.println("currentPacketScanValue: " + currentPacketScanValue);
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

      Serial.println("thruster1Power: " + String(thruster1Power, FLOAT_DECIMAL_PLACES));
      Serial.println("thruster1Angle: " + String(thruster1Angle));
      Serial.println("=====================\n\n");
      
      packetString = "";
    }
    else
    {
      packetString += incomingChar;
    }
  }
}
