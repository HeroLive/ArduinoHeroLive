#include <math.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//driver for the axis 2 - Y
#define PUL2_PIN 3
#define DIR2_PIN 6
//driver for the axis 3 - Z
#define PUL3_PIN 4
#define DIR3_PIN 7
//enable pin for the axis 1,2,3
#define EN1_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, PUL3_PIN, DIR3_PIN);
MultiStepper steppers;

boolean en = false;
const byte numChars = 100;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
float positions[3] = {0, 0, 0};
int microStep[3] = {16, 16, 16};
float angleStep[3] = {1.8, 1.8, 1.8};
long curSpeed = 100;

void setup()
{
  Serial.begin(9600);
  delay(100);

  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(PUL3_PIN, OUTPUT);
  pinMode(DIR3_PIN, OUTPUT);

  pinMode(EN1_PIN, OUTPUT);

  digitalWrite(DIR1_PIN, LOW);
  digitalWrite(DIR2_PIN, LOW);
  digitalWrite(DIR3_PIN, LOW);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);

  enableAll();

}

void loop()
{
  recvWithEndMarker();
  parseData(receivedChars);
}

void parseData(char * receivedChars_) {
  char g[5] = "";
  String g_ = "";
  char chars[10] = "";
  String stringValue = "";
  const char delim[2] = " ";
  if (newData == false) {
    return;
  }
  Serial.write(receivedChars_);
  Serial.write("\n");
  newData = false;
  // split the data into its parts

  char * token; // this is used by strtok() as an index

  token = strtok(receivedChars_, delim);     // get the first part - the string
  strcpy(g, token); //
  g_ = String(g);
  while (token != NULL && g_.startsWith("G10") ) {
    token  = strtok(NULL, delim); // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    if (stringValue.startsWith("X")) {
      positions[0] = toPosition(stringValue);
    } else if (stringValue.startsWith("Y")) {
      positions[1] = toPosition(stringValue);
    } else if (stringValue.startsWith("Z")) {
      positions[2] = toPosition(stringValue);
    }
  }

  //set speed, en, disable
  while (token != NULL && g_.startsWith("G20")) {
    token  = strtok(NULL, delim); // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    if (stringValue.startsWith("F")) {
      curSpeed = microStep[0] * toPosition(stringValue);
    } else if (stringValue.startsWith("E0")) {
      disableAll();
    } else if (stringValue.startsWith("E1")) {
      enableAll();
    }
  }

  if (g_.startsWith("G10")) {
    runSteppers(positions);
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

void runSteppers(float pos[3])
{
  long steps[3] = {};
  stepper1.setMaxSpeed(curSpeed);
  stepper2.setMaxSpeed(curSpeed);
  stepper3.setMaxSpeed(curSpeed);
  for (int i=0;i<3;i++){
    steps[i] = pos[i] * microStep[i] / angleStep[i];
  }
  if (en) {
    steppers.moveTo(steps);
    steppers.runSpeedToPosition();
  }
}
float toPosition(String s) {
  String dataInS = s.substring(1, s.length());
  char buff[dataInS.length() + 1];
  dataInS.toCharArray(buff, dataInS.length() + 1);
  return atof(buff);
}
void enableAll() {
  digitalWrite(EN1_PIN, LOW);
  //  Serial.write("Enable");
  //  Serial.write("\n");
  en = true;
}
void disableAll() {
  digitalWrite(EN1_PIN, HIGH);
  //  Serial.write("Disable");
  //  Serial.write("\n");
  en = false;
}
