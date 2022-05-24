#include <Arduino.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiManager.h>         // v0.16.0 https://github.com/tzapu/WiFiManager

#include <SocketIoClient.h>
/*v0.3 fix error beginSSL
  - SocketIoClient.cpp: const char* fingerprint --> const uint8_t* fingerprint
  - SocketIoClient.h:  const char* fingerprint = DEFAULT_FINGERPRINT)--> const uint8_t* fingerprint = NULL
*/
#include <WiFiUdp.h> //realtime
#include <NTPClient.h> //realtime

ESP8266WiFiMulti WiFiMulti;
SocketIoClient socketIO;

//properties realtime from internet: vn.pool.ntp.org
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "vn.pool.ntp.org");

char host[] = "192.168.1.4";
int port = 3484;
//char host[] = "dht-led.glitch.me";
//int port = 80;
char username[] = "esp";
char password[] = "1234";

uint64_t messageTimestamp;

#define DHT11Pin D1
#define DHTType DHT11
DHT HT(DHT11Pin, DHTType);
double humi;
double tempC;
double tempF;
long count = 0;
unsigned long epochTime;
StaticJsonDocument<200> SensorDoc;

void setup() {
  Serial.begin(115200);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //  wifiManager.resetSettings();

  //set custom ip for portal
  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect(username, password);
  //or use this for auto generated name ESP + ChipID
  //  wifiManager.autoConnect();
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  socketIO.begin(host, port);
  // use HTTP Basic Authorization this is optional remove if not needed
  //  socketIO.setAuthorization("username", "password");

  HT.begin();
  delay(1000);
  dht();

  //Realtime setup
  timeClient.begin();
  timeClient.setTimeOffset(7 * 3600); //Vietnam Timezone
}

void loop() {
  epochTime = getTime();
  socketIO.loop();
  if (millis() - messageTimestamp > 5000) {
    messageTimestamp = millis();
    count++;
    dht();
  }
}

void dht() {
  humi = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);

  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  SensorDoc["dht"]["tempC"] = tempC;
  SensorDoc["dht"]["humi"] = humi;
  SensorDoc["dht"]["count"] = count;
  SensorDoc["dht"]["date"] = epochTime;
  char msg[256];
  serializeJson(SensorDoc, msg);
  socketIO.emit("sensor2Server", msg);
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}
