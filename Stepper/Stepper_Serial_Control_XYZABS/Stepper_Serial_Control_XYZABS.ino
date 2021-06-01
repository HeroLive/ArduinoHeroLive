#include <math.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <stdlib.h>
#include <Servo.h>

//driver for the axis 1 - X - X
#define PUL1_PIN A0
#define DIR1_PIN A1
//driver for the axis 2 - Y - Y
#define PUL2_PIN A6
#define DIR2_PIN A7
//driver for the axis 3  - Z  - Z
#define PUL3_PIN 46
#define DIR3_PIN 48
//driver for the axis 4 - E0  - A
#define PUL4_PIN 26
#define DIR4_PIN 28
//driver for the axis 5   E1 - B
#define PUL5_PIN 36
#define DIR5_PIN 34

//enable pin for the axis 1 - 5
#define EN1_PIN 38
#define EN2_PIN A2
#define EN3_PIN A8
#define EN4_PIN 24
#define EN5_PIN 30

//servo
#define SERVO_PIN 4

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, PUL3_PIN, DIR3_PIN);
AccelStepper stepper4(AccelStepper::DRIVER, PUL4_PIN, DIR4_PIN);
AccelStepper stepper5(AccelStepper::DRIVER, PUL5_PIN, DIR5_PIN);
MultiStepper steppers;
Servo servo;  // create servo object to control a servo

boolean en = false;
const byte numChars = 100;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
float positions[6] = {0, 0, 0, 0, 0, 0};
int microStep[6] = {16, 16, 16, 16, 16, 1};
float angleStep[6] = {1.8, 1.8, 1.8, 1.8, 1.8, 1.0};
long curSpeed = 100;

void setup()
{
  Serial2.begin(9600); //for bluetooth
  Serial.begin(115200);
  delay(100);

  servo.attach(SERVO_PIN);

  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(PUL3_PIN, OUTPUT);
  pinMode(DIR3_PIN, OUTPUT);
  pinMode(PUL4_PIN, OUTPUT);
  pinMode(DIR4_PIN, OUTPUT);
  pinMode(PUL5_PIN, OUTPUT);
  pinMode(DIR5_PIN, OUTPUT);

  pinMode(EN1_PIN, OUTPUT);
  pinMode(EN2_PIN, OUTPUT);
  pinMode(EN3_PIN, OUTPUT);
  pinMode(EN4_PIN, OUTPUT);
  pinMode(EN5_PIN, OUTPUT);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
  steppers.addStepper(stepper5);

  enableAll();
  runSteppers(positions);

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
  Serial2.write(receivedChars_);
  Serial2.write("\n");
  Serial.println(receivedChars_);
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
    } else if (stringValue.startsWith("A")) {
      positions[3] = toPosition(stringValue);
    } else if (stringValue.startsWith("B")) {
      positions[4] = toPosition(stringValue);
    } else if (stringValue.startsWith("S")) {
      positions[5] = toPosition(stringValue);
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
  while (Serial2.available() > 0 && newData == false) {
    rc = Serial2.read();

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
  long steps[6] = {};
  stepper1.setMaxSpeed(curSpeed);
  stepper2.setMaxSpeed(curSpeed);
  stepper3.setMaxSpeed(curSpeed);
  stepper4.setMaxSpeed(curSpeed);
  stepper5.setMaxSpeed(curSpeed);
  for (int i = 0; i < 6; i++) {
    steps[i] = pos[i] * microStep[i] / angleStep[i];
  }
  if (en) {
    servo.write(steps[5]);
    steppers.moveTo(steps);
    steppers.runSpeedToPosition();
  }
  for (int i = 0; i < 6; i++) {
    Serial.print(pos[i]);
    Serial.print("\t");
  }
  Serial.println("");
}
float toPosition(String s) {
  String dataInS = s.substring(1, s.length());
  char buff[dataInS.length() + 1];
  dataInS.toCharArray(buff, dataInS.length() + 1);
  return atof(buff);
}
void enableAll() {
  // all axis enable
  Serial.println("Enable all steppers");
  digitalWrite(EN1_PIN, LOW);
  digitalWrite(EN2_PIN, LOW);
  digitalWrite(EN3_PIN, LOW);
  digitalWrite(EN4_PIN, LOW);
  digitalWrite(EN5_PIN, LOW);
  en = true;
}
void disableAll() {
  // all axis disabled!
  Serial.println("Disable all steppers");
  digitalWrite(EN1_PIN, HIGH);
  digitalWrite(EN2_PIN, HIGH);
  digitalWrite(EN3_PIN, HIGH);
  digitalWrite(EN4_PIN, HIGH);
  digitalWrite(EN5_PIN, HIGH);
  en = false;
}
