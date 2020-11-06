#include <Servo.h>
int SPin = 2;
int XPin = A0;
int YPin = A1;
int XSPin = 3;
int YSPin = 5;
int SVal;
int XVal;
int YVal;
int XPos=0;
int YPos=0;
Servo XServo;
Servo YServo;
void setup() {
  // put your setup code here, to run once:
  pinMode(SPin,INPUT_PULLUP);
  pinMode(XPin,INPUT);
  pinMode(YPin,INPUT);
  XServo.attach(XSPin);
  YServo.attach(YSPin);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  XVal = analogRead(XPin);
  YVal = analogRead(YPin);
  SVal = digitalRead(SPin);

  XPos = map(XVal,0,1023,0,180);
  YPos = map(YVal,0,1023,0,180);

  XServo.write(XPos);
  YServo.write(YPos);

  delay(300);
  Serial.print("X: ");
  Serial.print(XPos);
  Serial.print(" Y: ");
  Serial.print(YPos);
  Serial.print(" SW: ");
  Serial.println(SVal);
}
