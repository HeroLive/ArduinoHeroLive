//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>

// You should get Auth Token in the Blynk App.
char auth[] = "iKvG0uSG1DireTrH9ltelbI44DbdqXrl";

void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("ESP8266");
  //if you get here you have connected to the WiFi
  Serial.println("connected...ok :)");
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop()
{
  Blynk.run();  // Commented out for a short time.
}
