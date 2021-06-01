#include <math.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <stdlib.h>
#include <SPI.h>
#include <SD.h>

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

File myFile;
const int chipSelect = 10;
String fileData = "data.txt";
int indexSD = 0;
String storedSdData[5];
String receivedString = "";

boolean en = false;
const byte numChars = 10;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
float positions[3] = {0, 0, 0};
int microStep[3] = {16, 16, 16};
float angleStep[3] = {1.8, 1.8, 1.8};
long curSpeed = 1600;

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

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
  } else {
    Serial.println("initialization done.");
//    runFromSD(1, fileData) ;
  }

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
  } else if (g_.startsWith("G50")) {
    sd_write(fileData, receivedString.substring(4, receivedString.length())) ;
  } else if (g_.startsWith("G51")) {
    Serial.println("Remove file ");
    indexSD = 0;
    SD.remove(fileData);
  } else if (g_.startsWith("G52")) {
    Serial.println("Read file from SD and run ");
    runFromSD(1, fileData) ;
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
  receivedString = String(receivedChars);
}

void runSteppers(float pos[3])
{

  long steps[3] = {};
  stepper1.setMaxSpeed(curSpeed);
  stepper2.setMaxSpeed(curSpeed);
  stepper3.setMaxSpeed(curSpeed);
  for (int i = 0; i < 3; i++) {

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
void sd_write(String file, String s) {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another
  indexSD++;
  Serial.println(indexSD);
  myFile = SD.open(file, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to file ...");
    myFile.println(s);
    // close the file:
    myFile.close();
    Serial.println(" done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening...");
  }
}
void sd_read_data(String file) {
  // re-open the file for reading:
  int index_ = 0;
  String buffer;
  myFile = SD.open(file);
  if (myFile) {
    Serial.println("Open file to read");

    // read from the file until there's nothing else in it:
    Serial.println("Read and save to array");
    while (myFile.available()) {
      index_++;
      buffer = myFile.readStringUntil('\n');
      Serial.println(buffer);
      storedSdData[index_] = buffer;
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
  }
  indexSD = index_;
}

void runFromSD(int start, String file) {
  Serial.println(file);
  sd_read_data(file);
  Serial.println("Run from save SD");
  for (int i = start; i <= indexSD; i++) {
    char chars[storedSdData[i].length() + 1];
    strcpy(chars, storedSdData[i].c_str());
    newData = true;
    Serial.print(i);
    Serial.print("\t");
    parseData(chars);
    delay(200);
  }
}
