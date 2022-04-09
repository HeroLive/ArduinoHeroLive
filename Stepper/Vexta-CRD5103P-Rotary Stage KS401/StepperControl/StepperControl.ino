int dirPin = 11 ;
int stepPin = 10 ;
long delayTime = 500;
void setup()
{
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT); // Step
  pinMode(dirPin, OUTPUT); // Dir
  delay(3000);
}
void loop() {
  digitalWrite(dirPin, LOW);
  for (int i = 1 ; i <= 18750 ; i = i + 1) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
  delay(2000);  
  digitalWrite(dirPin, HIGH);
  for (int i = 1 ; i <= 12500 ; i = i + 1) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
  delay(1000);
}
