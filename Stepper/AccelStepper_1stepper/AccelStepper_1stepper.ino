#include <AccelStepper.h>
//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//driver for the axis 2 - Y
#define PUL2_PIN 3
#define DIR2_PIN 6
#define EN_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);

long positions[2] = {800, 200}; // Array of desired stepper positions

void setup()
{
  Serial.begin(9600);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  stepper1.setMaxSpeed(32000.0);
  stepper1.setAcceleration(1000.0);
}

void loop()
{
  stepper1.runToNewPosition(0);
  delay(2000);
  stepper1.runToNewPosition(2 * positions[0]);
  delay(5000);
  stepper2.runToNewPosition(0);
  delay(2000);
  stepper2.runToNewPosition(2 * positions[0]);
  delay(5000);
}
