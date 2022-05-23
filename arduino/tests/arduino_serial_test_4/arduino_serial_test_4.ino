char incomingByte;
String incomingString;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    if(incomingByte != '\n')
    {
      incomingString += incomingByte;
    }
    else
    {
      Serial.print(incomingString);
      incomingString = "";
    }
  }
}
