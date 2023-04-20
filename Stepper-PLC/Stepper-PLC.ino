#include <math.h>
#include "StepperMotor.h"
#define PUL1_PIN 2
#define DIR1_PIN 5
#define HOME_PIN 9
#define HOME_BUTTON_PIN 10
StepperMotor motor_01(PUL1_PIN, DIR1_PIN);

bool debug = true;
bool M0 = false;
long D206 = 8000;  //home speed
long D208 = 500;  //creep speed


void setup() {
  Serial.begin(9600);
  pinMode(HOME_BUTTON_PIN, INPUT_PULLUP);
  pinMode(HOME_PIN, INPUT_PULLUP);
  delay(1000);
}

void loop() {
  if (digitalRead(HOME_BUTTON_PIN) == 0) {
    M0 = true;
  }
  if (M0) {
    motor_01.ZRN(D206, D208, HOME_PIN);
    if (motor_01.getExeCompleteFlag()) {
      M0 = false;
      if(debug){
        Serial.print("Come home - Position: ");
      Serial.println(motor_01.getCurrentPosition());
      }
      
    }
    // Serial.println(motor_01.getCurrentPosition());
  }
}
