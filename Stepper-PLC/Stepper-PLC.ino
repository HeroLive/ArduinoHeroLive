#include <math.h>
#include "StepperMotor.h"
#define PUL1_PIN 2
#define DIR1_PIN 5
#define PUL2_PIN 3
#define DIR2_PIN 6
#define BT1_PIN 12
StepperMotor motor_01(PUL1_PIN, DIR1_PIN);
StepperMotor motor_02(PUL2_PIN, DIR2_PIN);


bool M1 = false;
long D200 = 40000; //motor 2 frequency
long D202 = 100; //motor 2 pulse value
long D204 = 5000; //motor 2 interval time
long D0 = 0; //target position
long _t =millis() ;


void setup() {
  Serial.begin(115200);
  pinMode(BT1_PIN,INPUT_PULLUP);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // motor_01.PLSV(10000);
  // if (digitalRead(13)==0){
  //   M1 = true;
  // }
  if(millis() - _t >= D204){
    _t = millis();
    M1 = true;
    D0 = D0 == 0? 32000 : 0;
    Serial.print("Target: ");
    Serial.println(D0);
  }
  if (M1){
    motor_02.DRVA(D0, D200);
    if(motor_02.getExeCompleteFlag()){
      M1 = false;
      Serial.println(motor_02.getCurrentPosition());
    }
  }
  
  // Serial.println(motor_02.getCurrentPosition());
}
