#include <Servo.h>
int echoPin = 11;
int trigPin = 10;
int servoPin = 9;
int duration; //micro second
float d_cm;
int pos01=0;
int pos02 = 90;
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
    myServo.write(pos02);
    delay(200);
  } else {
    myServo.write(pos01);
    delay(200);
  }
  //print
  Serial.print("Distance:");
  Serial.print(d_cm);
  Serial.println("cm");
}
