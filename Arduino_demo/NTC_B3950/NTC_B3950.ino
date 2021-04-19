//NTC B3950 Steinhart–Hart equation
int tempPin = A0;
float Vo;
float R1 = 10000;
float logR2, R2, T, temp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float tempC;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(tempPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tempC = get_temp();
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");
  delay(500);
}

float get_temp() {
  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  return T - 273.15; //độ C
}
