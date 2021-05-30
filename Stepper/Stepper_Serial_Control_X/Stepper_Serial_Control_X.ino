#include <math.h>
#include <AccelStepper.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//enable pin for the axis 1
#define EN1_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);

boolean en = false;
const byte numChars = 100;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
float positions[1] = {0};
int microStep[1] = {16};
float angleStep[1] = {1.8};
long curSpeed = 100;

void setup()
{
  Serial.begin(9600);
  delay(100);

  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);

  digitalWrite(DIR1_PIN, LOW);

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
    }
  }

  //set speed, en, disable
  while (token != NULL && g_.startsWith("G20")) {
    token  = strtok(NULL, delim); // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    if (stringValue.startsWith("F")) {
      curSpeed = microStep[0] * toPosition(stringValue);
      Serial.println(curSpeed);
    } else if (stringValue.startsWith("E0")) {
      disableAll();
    } else if (stringValue.startsWith("E1")) {
      enableAll();
    }
  }

  if (g_.startsWith("G10")) {
    runStepper1();
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

void runStepper1()
{
  long newPositions = positions[0] * microStep[0] / angleStep[0];
  long acc = 0.2 * curSpeed;
  if (en) {
    stepper1.setMaxSpeed(curSpeed);
    stepper1.setAcceleration(acc);
    stepper1.runToNewPosition(newPositions);
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
