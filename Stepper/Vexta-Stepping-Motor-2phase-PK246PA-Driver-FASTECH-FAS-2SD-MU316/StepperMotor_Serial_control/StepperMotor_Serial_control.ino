#include <StepperMotor.h> //https://github.com/HeroLive/StepperMotor
#define pul 10
#define dir 11

StepperMotor stepper(pul, dir);

float disPerRound = 1.0 ;// only round: 1.0 round, vitme T8-4mm: 4.0 mm, GT2 Pulley 16 Teeth: 16x2 = 32.0 mm
int microStep = 1; //1: full step, 2: haft step, ...
float angleStep = 1.8; //a step angle of 1.8 degrees with 200 steps per revolution
float stepsPerUnit = (1 / disPerRound) * 360.0 * microStep / angleStep; //steps/round or steps/mm ...
float target = 0;

float speed = 2.0; //set 2 round/s

const byte numChars = 50;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(9600);
  stepper.setStepsPerUnit(stepsPerUnit);
  stepper.setSpeed(speed);
  stepper.setStartDirection(LOW);
  delay(3000);
}

void loop() {
  recvWithEndMarker();
  parseData();
}

float toPosition(String s) {
  String dataInS = s.substring(1, s.length());
  char buff[dataInS.length() + 1];
  dataInS.toCharArray(buff, dataInS.length() + 1);
  return atof(buff);
}

void parseData() {
  char g[] = "";
  String g_ = "";
  char chars[] = "";
  String stringValue = "";
  const char delim[2] = " ";
  if (newData == false) {
    return;
  }
  Serial.println(receivedChars);
  newData = false;
  // split the data into its parts

  char * token; // this is used by strtok() as an index
  token = strtok(receivedChars, delim);
  strcpy(g, token); //
  g_ = String(g);

  while (token != NULL && g_.startsWith("G10")) {
    token  = strtok(NULL, delim); // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    if (stringValue.startsWith("X")) {
      target = toPosition(stringValue);
    }
  }
  while (token != NULL && g_.startsWith("G20")) {
    token  = strtok(NULL, delim); // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    if (stringValue.startsWith("F")) {
      speed = toPosition(stringValue);
    }
  }
  if (g_.startsWith("G10")) {
    stepper.setSpeed(speed);
    stepper.moveTo(target);
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
