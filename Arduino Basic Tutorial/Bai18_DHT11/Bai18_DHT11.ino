#include "DHT.h"
#define DHT11Pin 2
#define DHTType DHT11
DHT HT(DHT11Pin,DHTType);
float humi;
float tempC;
float tempF;

void setup() {
  Serial.begin(9600);
  HT.begin();
  delay(1000); 
}

void loop() {
 delay(1000);
 humi = HT.readHumidity();
 tempC = HT.readTemperature();
 tempF = HT.readTemperature(true);

 Serial.print("Humidity:");
 Serial.print(humi,0);
 Serial.print("%");
 Serial.print(" Temperature:");
 Serial.print(tempC,1);
 Serial.print("‎°C ~ ");
 Serial.print(tempF,1);
 Serial.println("°F");
 
}
