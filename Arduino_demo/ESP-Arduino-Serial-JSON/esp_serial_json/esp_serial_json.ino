#include <SoftwareSerial.h>

SoftwareSerial Serial_ESP(13, 15); //RX,TX

float floatval;
long timeTick=millis();

const byte numChars = 100;
char receivedChars[numChars];
boolean newData = false;

void setup()
{
  Serial.begin(9600);
  Serial_ESP.begin(9600);
  Serial_ESP.println("Esp Connect! - SOFTWARESERIAL");
  floatval = 0;
}

void loop() // run over and over
{
  if (millis() - timeTick > 1000) {
    timeTick = millis();
    floatval = floatval + 1;
    Serial_ESP.println(floatval, 4);
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
  while (Serial_ESP.available() > 0 && newData == false) {
    rc = Serial_ESP.read();

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
