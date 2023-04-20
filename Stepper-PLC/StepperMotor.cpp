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
bool StepperMotor::getExeCompleteFlag() {
  return exe_complete_flag;
}
void StepperMotor::PLSV(long f) {
  if (f == 0) return;
  interval = 1000000.0 / abs(f) * 0.5;
  if (micros() - t_us >= interval) {
    d_state = f < 0 ? false : true;
    p_state = !p_state;
    t_us = micros();
    digitalWrite(pp, p_state);
    digitalWrite(pd, d_state);
    if (p_state == false) {
      position = f > 0 ? position + 1 : position - 1;
    }
  }
}

void StepperMotor::ZRN(long fr, long fc, int ns) {
  if (!pulse_output_flag) {
    interval = 1000000.0 / abs(fr) * 0.5;
    d_state = false;
    pinMode(ns, INPUT_PULLUP);
    digitalWrite(pd, d_state);
    exe_complete_flag = false;
  }

  if (micros() - t_us >= interval) {
    p_state = !p_state;
    t_us = micros();
    digitalWrite(pp, p_state);
    pulse_output_flag = true;
    if (p_state == false) {
      position = position - 1;
    }
  }
  if (digitalRead(ns) == 0) {
    delay(10);
    interval = 1000000.0 / abs(fc) * 0.5;
    digitalWrite(pd, d_state);
    while (digitalRead(ns) == 0) {
      if (micros() - t_us >= interval) {
        p_state = !p_state;
        t_us = micros();
        digitalWrite(pp, p_state);
        pulse_output_flag = true;
        if (p_state == false) {
          position = position + 1;
        }
      }
    }
    pulse_output_flag = false;
    exe_complete_flag = true;
    position = 0;
  }
}

void StepperMotor::DRVI(long p, long f) {
  if (!pulse_output_flag) {
    increment = 0;
  }
  if (f == 0 || p == 0 || p == increment) {
    exe_complete_flag = true;
    pulse_output_flag = false;
    return;
  } else {
    exe_complete_flag = false;
  }
  interval = 1000000.0 / abs(f) * 0.5;
  if (micros() - t_us >= interval) {
    d_state = p < 0 ? false : true;
    p_state = !p_state;
    t_us = micros();

    digitalWrite(pp, p_state);
    digitalWrite(pd, d_state);

    pulse_output_flag = true;

    if (p_state == false) {
      increment = p > 0 ? increment + 1 : increment - 1;
      position = p > 0 ? position + 1 : position - 1;
    }
  }
}
void StepperMotor::DRVA(long p, long f) {
  if (f == 0 || p == position) {
    exe_complete_flag = true;
    pulse_output_flag = false;
    return;
  } else {
    exe_complete_flag = false;
  }
  interval = 1000000.0 / abs(f) * 0.5;
  if (micros() - t_us >= interval) {
    d_state = p - position < 0 ? false : true;
    p_state = !p_state;
    t_us = micros();

    digitalWrite(pp, p_state);
    digitalWrite(pd, d_state);

    pulse_output_flag = true;

    if (p_state == false) {
      position = p - position < 0 ? position - 1 : position + 1;
    }
  }
}
