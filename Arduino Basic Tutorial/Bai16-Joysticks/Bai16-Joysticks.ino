int SPin = 2;
int XPin = A0;
int YPin = A1;
int SVal;
int XVal;
int YVal;
void setup() {
  // put your setup code here, to run once:
  pinMode(SPin,INPUT_PULLUP);
  pinMode(XPin,INPUT);
  pinMode(YPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  XVal = analogRead(XPin);
  YVal = analogRead(YPin);
  SVal = digitalRead(SPin);

  delay(300);
  Serial.print("X: ");
  Serial.print(XVal);
  Serial.print(" Y: ");
  Serial.print(YVal);
  Serial.print(" SW: ");
  Serial.println(SVal);
}
