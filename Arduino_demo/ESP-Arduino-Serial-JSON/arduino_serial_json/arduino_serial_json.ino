#include <SoftwareSerial.h>


SoftwareSerial Serial_MEGA(10, 11); //RX,TX

float floatval;
long timeTick = millis();

const byte numChars = 100;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(9600);
  Serial_MEGA.begin(9600);
  Serial_MEGA.println("Mega Connect! - SOFTWARESERIAL");
  floatval = 0;
}

void loop() {
  if (millis() - timeTick > 5000) {
    timeTick = millis();
    floatval = floatval + 5;
    Serial_MEGA.println(floatval,0);
  }
  recvWithEndMarker();
  if (newData == false) {
    return;
  } else {
    Serial.println(receivedChars);
    newData = false;
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial_MEGA.available() > 0 && newData == false) {
    rc = Serial_MEGA.read();

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
