int i = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("i value: " + String(i) + "\n");
  delay(1100);
  i++;
}
