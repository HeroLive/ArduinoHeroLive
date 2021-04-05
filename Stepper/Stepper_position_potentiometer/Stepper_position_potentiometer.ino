// ProportionalControl.pde
// -*- mode: C++ -*-
//
// Make a single stepper follow the analog value read from a pot or whatever
// The stepper will move at a constant speed to each newly set posiiton, 
// depending on the value of the pot.
//
// Copyright (C) 2012 Mike McCauley
// $Id: ProportionalControl.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
#include <AccelStepper.h>
// Define a stepper and the pins it will use
int stepPin = 2;
int dirPin = 5;
int en = 8;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// This defines the analog input pin for reading the control voltage
// Tested with a 10k linear pot between 5v and GND
#define ANALOG_IN A0
int analog_in = 0;


void setup()
{  
  Serial.begin(9600);
  pinMode(en,OUTPUT);
  digitalWrite(en,LOW);
  stepper.setMaxSpeed(1000);
}

void loop()
{
  // Read new position
  analog_in = map(analogRead(ANALOG_IN),0,1023,0,799);
  Serial.println(analog_in);
  stepper.moveTo(analog_in);
  stepper.setSpeed(100);
  stepper.runSpeedToPosition();
}
