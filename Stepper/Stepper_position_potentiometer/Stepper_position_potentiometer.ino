#include <AccelStepper.h>
// Define a stepper and the pins it will use
int stepPin = 2;
int dirPin = 5;
int en = 8;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// This defines the analog input pin for reading the control voltage
// Tested with a 10k linear pot between 5v and GND

int curPot = 0;
int lastPot = 0; // current potentiometer value, last potentiometer value
int potPin = A0; // potentiometer is connected to analog port A0
int curStep = 0; // the current step 
int curDegree = 0; // will store the degree equivalent of the potentiometer reading
int microStep = 800;
void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);
  stepper.setSpeed(100);
  stepper.setMaxSpeed(8000);
  stepper.setAcceleration(500); 
}

void loop()
{
  // Read new position
  curPot = analogRead(potPin); // read the potentiometer
  if (abs(curPot - lastPot) > 5) { // if the potentiometer hasn't moved more than 5
    // values
    curDegree = map(curPot, 0, 1023, 0, 360); // update values and set the stepper motor
    // to the new direction
    curStep = map(curDegree, 0, 360, 0, microStep);
    Serial.print(curPot);
    Serial.print("\t");
    Serial.print(curDegree);
    Serial.print("\t");
    Serial.println(curStep);
    stepper.moveTo(curStep);// set the stepper motor to the current step
    lastPot = curPot; // reset the last pot value to the current pot value
  }
  stepper.run();// execute the motor step change
  delay(5); // slight delay for sketch (Don't go lower than 2. Motor may hang)
}
