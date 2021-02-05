#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define MotorASpeed 5
#define MotorADir   0 // HIGH --> A+ -> Motor+, A- -> Motor-

char auth[] = "Your token";
char ssid[] = "Wifi_name";
char pass[] = "Password";

void moveControl(int x)
{
  digitalWrite(MotorADir, HIGH);
  analogWrite(MotorASpeed, x);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  // initial settings for motors off and direction forward
  pinMode(MotorASpeed, OUTPUT);
  pinMode(MotorADir, OUTPUT);
  
  digitalWrite(MotorADir, HIGH);
  moveControl(0);
}


void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V1)
{
  int x = param.asInt();
  
  Serial.print("x value is: ");
  Serial.println(x);
  
  moveControl(x);
}
