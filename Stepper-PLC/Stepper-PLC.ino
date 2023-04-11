#include <math.h>
#include "StepperMotor.h"
#define PUL1_PIN 2
#define DIR1_PIN 5
#define PUL2_PIN 3
#define DIR2_PIN 6

StepperMotor motor_02(PUL2_PIN, DIR2_PIN);
void setup() {
  // put your setup code here, to run once:
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:

  motor_02.DRVI(10, 100);
}
