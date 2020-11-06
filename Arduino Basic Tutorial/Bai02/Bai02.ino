void setup() {
  // put your setup code here, to run once:
pinMode(8,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8,HIGH);
  delay(50);
  digitalWrite(8,LOW);
  delay(50);
}
