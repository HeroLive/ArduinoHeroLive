//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>

#define bt D8

// You should get Auth Token in the Blynk App.
char auth[] = "Your token";
WiFiManager wifiManager;
void setup()
{
  long t = millis();
  pinMode(bt, INPUT); //bt wire D8 and 3.3V
  EEPROM.begin(512);
  Serial.begin(9600);

  //Waitting 3s to join wipe wifi when press button
  delay(3000);
  if (digitalRead(bt) == 1) {
    wifiManager.resetSettings();
    delay(1000);
  }

  //  wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("ESP8266");
  //if you get here you have connected to the WiFi
  Serial.println("connected...ok :)");
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop()
{
  Serial.println(digitalRead(bt));
  //  Blynk.run();  // Commented out for a short time.
}
