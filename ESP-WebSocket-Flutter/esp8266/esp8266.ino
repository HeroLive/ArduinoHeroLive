#include <Arduino.h>
#include <WebSocketsServer.h> //import for websocket
#include <ArduinoJson.h> //data Json
#include <StepperMotor.h> //https://github.com/HeroLive/StepperMotor

#define pul D1
#define dir D2
#define LED D0

const byte numChars = 100;
char receivedChars[numChars];

const char *ssid =  "esp-motor";   //Wifi SSID (Name)
const char *pass =  "123456789"; //wifi password
uint8_t ipClient;
WebSocketsServer webSocket = WebSocketsServer(81); //websocket init with port 81

StaticJsonDocument<500> DataDoc;

StepperMotor stepper(pul, dir);
float stepsPerUnit = 200;
float target = 0;
float speed = 2.0; //set 2 round/s

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //serial start
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  Serial.println("Connecting to wifi");

  IPAddress apIP(192, 168, 99, 99);   //Static IP for wifi gateway
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //set Static IP gateway on NodeMCU
  WiFi.softAP(ssid, pass); //turn on WIFI

  webSocket.begin(); //websocket Begin
  webSocket.onEvent(webSocketEvent); //set Event for websocket
  Serial.println("Websocket is started");

  stepper.setStepsPerUnit(stepsPerUnit);
  stepper.setSpeed(speed);
  stepper.setStartDirection(LOW);
  delay(3000);
}

void loop() {  
  webSocket.loop(); //keep this line on loop method
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  //webscket event method
  String cmd = "";
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        String msg = "";
        Serial.println("Websocket is connected");
        Serial.println(webSocket.remoteIP(num).toString());
        ipClient = num;
        webSocket.sendTXT(num, "connected");
        delay(500);

//        char msg[256];
//        serializeJson(DataDoc, msg);
//        webSocket.sendTXT(num, msg);
//        delay(100);
      }
      break;
    case WStype_TEXT:
      digitalWrite(LED, LOW);
      cmd = "";
      for (int i = 0; i < length; i++) {
        cmd = cmd + (char) payload[i];
      } //merging payload to single string
      Serial.println(cmd);
      ParseDataEvent(cmd);
      digitalWrite(LED, HIGH);
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

void ParseDataEvent(String json) {
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(DataDoc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.print("stepPerUnit: ");
  Serial.print(String(DataDoc["stepPerUnit"]));
  Serial.print(" Speed: ");
  Serial.print(String(DataDoc["speed"]));
  Serial.print(" Position: ");
  Serial.println(String(DataDoc["pos"]));

  stepsPerUnit = DataDoc["stepPerUnit"];
  speed = DataDoc["speed"];
  target = DataDoc["pos"];

  stepper.setStepsPerUnit(stepsPerUnit);
  stepper.setSpeed(speed);
  stepper.moveTo(target);
}
