#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
void setup()
{
  WiFiManager wifiManager;
  WiFi.disconnect();
  wifiManager.resetSettings();
  ESP.eraseConfig();
  ESP.reset();
  ESP.restart();

  delay(1000);

  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  wifiManager.autoConnect();
}
void loop()
{
  yield();
}
