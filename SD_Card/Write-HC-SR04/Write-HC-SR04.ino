/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

int trigPin = 3;
int echoPin = 2;
int btSave = 4;
float duration, cm, inches;
String dataString = "";
String fileName = "data.txt";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(btSave,INPUT_PULLUP);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file for reading:
  myFile = SD.open(fileName);
  if (myFile) {
    Serial.println(fileName);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
  }

  // delete the file:

  Serial.println("Removing...");
  SD.remove(fileName);

  writeToSD(fileName, "Start write data to file");

}

void loop() {
  // nothing happens after setup
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  dataString = String(cm);
  Serial.print(cm);
  Serial.println(" cm");

  if(digitalRead(btSave)==0){
    while(digitalRead(btSave)==0);
    writeToSD(fileName, dataString);
  }

  delay(250);
}

void writeToSD(String fileName, String str) {
  File dataFile = SD.open(fileName, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(str);
    dataFile.close();
    // print to the serial port too:
    Serial.print("Saved: ");
    Serial.println(str);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
}
