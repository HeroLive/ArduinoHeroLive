#include <Servo.h>
int echoPin = 12;
int trigPin = 11;
int servoPin = 10;
int duration; //micro second
float d_cm;
int pos01=0;
int pos02 = 100;
Servo myServo;
void setup() {

// initialize serial communication:
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  myServo.attach(servoPin);
  myServo.write(pos01);
}

void loop() {
  // generate pusle trigger
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  // receive 
  duration = pulseIn(echoPin,HIGH);
  d_cm = duration * 0.034/2.;
  //servo
  if(d_cm>=11 & d_cm<=14){
    for (int i=0;i<=pos02; i+=10){
      myServo.write(i);
      delay(15);
    }
    delay(400);
    myServo.write(pos01);
    delay(1000);
  }
  //print
  Serial.print("Distance:");
  Serial.print(d_cm);
  Serial.println("cm");
}
