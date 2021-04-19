#include <DallasTemperature.h>
#include <OneWire.h>
// Data wire is plugged into pin D3 on the ESP8266
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature DS18B20(&oneWire);

float tempC;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  getTemperature();
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");
  delay(500);
}

void getTemperature() {
  do {
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    if (tempC == (-127)) {
      delay(100);
    }
  } while (tempC == (-127.0));
}
