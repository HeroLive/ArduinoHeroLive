#include <Servo.h>
Servo myServo;
int servoPin = 3;
int servoPos = 0;
int readPos = 0;
int readVal = 0;
int speedVal = 100; //max speed
int potPin = A0;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(servoPin);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  readVal = analogRead(potPin);
  speedVal = 100 - map(readVal, 0, 1023, 0, 100);

  //
  servoMove(0, 180, speedVal);
  

  delay(1000);
  //
  servoMove(180, 0, speedVal);


  delay(1000);
}

void servoMove(int _posA, int _posB, int _speed) {
  Serial.print(_posA);
  Serial.print(" --> ");
  Serial.print(_posB);
  Serial.print(" Pos: ");
  
  int d_pos = _posB - _posA;
  for (int i = 0; i <= abs(d_pos) ; i++) {
    if (d_pos >= 0) {
      myServo.write(_posA + i);
    } else {
      myServo.write(_posA - i);
    }
    delay(_speed);
    readPos = myServo.read();
    Serial.print(readPos);
    Serial.print("-");
  }

  Serial.println();
  
}
