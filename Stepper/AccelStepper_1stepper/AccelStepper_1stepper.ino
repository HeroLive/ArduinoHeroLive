#include <AccelStepper.h>
#include <MultiStepper.h>
//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//driver for the axis 2 - Y
#define PUL2_PIN 3
#define DIR2_PIN 6
#define EN_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);
MultiStepper steppers;

long positions[2] = {32000, 32000}; // Array of desired stepper positions

void setup()
{
  Serial.begin(9600);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(32000);
}

void loop()
{
  long positions[2]; // Array of desired stepper positions

  positions[0] = 8000;
  positions[1] = 640000;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(2000);

  // Move to a different coordinate
  positions[0] = 0;
  positions[1] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(2000);
}
