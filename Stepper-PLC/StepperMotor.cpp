#include "Arduino.h"
#include "StepperMotor.h"

StepperMotor::StepperMotor(int p, int d) : pp(p), pd(d)
{
  _time = micros();
    pinMode(pp, OUTPUT);
    pinMode(pd, OUTPUT);
}
long StepperMotor::getCurrentPosition(){
    return positon;
}
bool StepperMotor::getDirection(){
  return direction;
}
void StepperMotor::PLSV(long f)
{
  double t_f = 1/f * 1000000;
    if (1){}
}
