#include <TM1637Display.h>
//HC-SR04
#define echoPin  2
#define trigPin  3
#define resetPin 4

// 4-digital display pins (Digital Pins)
#define CLK 8
#define DIO 9
TM1637Display display(CLK, DIO);

float duration; //micro second
float d_cm;
long numDisplay=100;
boolean trigUp = false;
boolean trigDown = false;
float counterPushUp=0;

void setup() {

// initialize serial communication:
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  //
  pinMode(resetPin,INPUT_PULLUP);
  //
  display.setBrightness(4);
  display.clear();
  delay(500);
  display.showNumberDecEx(numDisplay, false, true, 4, 0);
}

void loop() {
  
  //reset by switch
  if(digitalRead(resetPin)==0){
    trigUp=false;
    trigDown=false;
    counterPushUp=0;
  }
  
  // generate pusle trigger
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  // receive, convert time (us) to cm
  duration = pulseIn(echoPin,HIGH);
  d_cm = duration * 0.034/2.;
  
  //trigger body 
  if (d_cm>15 && d_cm<=30){
    trigUp = true;
  } else if (d_cm < 10){
    trigDown = true;
  } else if(d_cm >30) {
  }
  //counter
  if (trigUp==true && trigDown==true){
    counterPushUp=counterPushUp+0.5;
    trigUp=false;
    trigDown=false;
  }
  //print
//  Serial.print(d_cm);
//  Serial.print(" --- ");
//  Serial.println(counterPushUp);
  display.showNumberDecEx(counterPushUp, false, true, 4, 0);
}
