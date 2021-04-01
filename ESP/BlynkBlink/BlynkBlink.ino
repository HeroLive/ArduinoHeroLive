//DEMO ESP8266 - BLYNK
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "user-Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wifi_name";
char pass[] = "pass";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

// This is called when Smartphone App is opened
BLYNK_APP_CONNECTED() {
  Serial.println("App Connected.");
}

// This is called when Smartphone App is closed
BLYNK_APP_DISCONNECTED() {
  Serial.println("App Disconnected.");
}
