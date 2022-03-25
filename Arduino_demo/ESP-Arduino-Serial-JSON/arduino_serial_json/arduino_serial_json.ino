#include <SoftwareSerial.h>
#include "DHT.h"
#include <ArduinoJson.h>

#define DHT11Pin 3
#define DHTType DHT11
DHT HT(DHT11Pin, DHTType);
float humi;
float tempC;
float tempF;
long count = 0;

SoftwareSerial Serial_MEGA(10, 11); //RX,TX

float floatval;
long timeTick = millis();

const byte numChars = 100;
char receivedChars[numChars];
boolean newData = false;

StaticJsonDocument<200> SensorDoc;

void setup() {
  Serial.begin(9600);
  Serial_MEGA.begin(9600);
  delay(1000);
  Serial_MEGA.println("Mega Connect! - SOFTWARESERIAL");
  floatval = 0;

  HT.begin();
  dht();
  Serial.println("Arduino Mega started");
}

void loop() {
  if (millis() - timeTick > 5000) {
    timeTick = millis();
    
//    floatval = floatval + 5;
//    Serial_MEGA.println(floatval,0);

    count++;
    dht();
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

void dht() {
  humi = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);
  SensorDoc["dht"]["tempC"] = round(tempC);
  SensorDoc["dht"]["humi"] = round(humi);
  SensorDoc["dht"]["count"] = count;
  char msg[256];
  serializeJson(SensorDoc, msg);
  Serial_MEGA.println(msg);
}
