char incomingByte;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    Serial.println("Recieved: " + String(incomingByte));
  }
}
