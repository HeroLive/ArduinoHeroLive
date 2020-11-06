#include <Servo.h>
Servo myServo;
int servoPin = 3;
int servoPos = 0;
int readVal=0;
int potPin = A0;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(servoPin);
  pinMode(potPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  readVal = analogRead(potPin);
  
  servoPos = map(readVal,0,1023,0,180);
  Serial.println(servoPos);
  
  myServo.write(servoPos);
  
}
