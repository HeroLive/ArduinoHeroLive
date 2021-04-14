#include <AccelStepper.h>
#include <MultiStepper.h>
// Define a stepper and the pins it will use
int stepPinX = 2;
int dirPinX = 5;
int stepPinZ = 4;
int dirPinZ = 7;
int en = 8;
#define ANALOG_IN A0
AccelStepper stepperX(AccelStepper::DRIVER, stepPinX, dirPinX);; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperZ(AccelStepper::DRIVER, stepPinZ, dirPinZ);; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;
long positions[2];
long JointX[50] = {0, 800, 600, 800, 400, 800, 200, 800, 0};
long JointZ[50] = {0, 800, 600, 800, 400, 800, 200, 800, 0};
int index = 8;
void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);

  stepperX.setMaxSpeed(200.0);
  stepperZ.setMaxSpeed(200.0);

  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperZ);
}

void loop()
{
  for (int i = 0; i <= index ; i++) {
    Serial.println(i);
    positions[0] = JointX[i];
    positions[1] = JointZ[i];
    steppers.moveTo(positions);
    steppers.runSpeedToPosition(); // Blocks until all are in position
    delay(1000);
  }

}
