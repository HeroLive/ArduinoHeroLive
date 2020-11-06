//5V -- 1023; 0V -- 0;
int readPin = A0;
int readVal = 0;
float V = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(readPin,INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  readVal = analogRead(readPin);
  V = (5./1023.) * readVal;
  Serial.print(readVal);
  Serial.print(" ");
  Serial.print(V);
  Serial.println("V");
  delay(500);
}
