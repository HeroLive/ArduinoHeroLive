#include "Arduino.h"
#include "StepperMotor.h"

StepperMotor::StepperMotor(int p, int d)
  : pp(p), pd(d) {
  t_us = micros();
  pinMode(pp, OUTPUT);
  pinMode(pd, OUTPUT);
}
long StepperMotor::getCurrentPosition() {
  return position;
}
bool StepperMotor::getDirection() {
  return direction;
}
void StepperMotor::PLSV(long f) {
  if (f == 0) return;
  double interval = 1000000.0 / abs(f) * 0.5;
  if (micros() - t_us >= interval) {
    bool d_state = f < 0 ? false : true;
    p_state = !p_state;
    t_us = micros();
    digitalWrite(pp, p_state);
    digitalWrite(pd, d_state);
  }
}

void StepperMotor::DRVI(long p, long f) {
  if (f == 0 || p == 0 || p == position) return;
  double interval = 1000000.0 / abs(f) * 0.5;
  if (micros() - t_us >= interval) {
    bool d_state = p < 0 ? false : true;
    p_state = !p_state;
    t_us = micros();
    digitalWrite(pp, p_state);
    digitalWrite(pd, d_state);
    if (p_state == false) {
      position = p > 0 ? position + 1 : position - 1;
    }
  }
}
