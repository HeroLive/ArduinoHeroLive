//5V -- 1023; 0V -- 0;
int readPin = A0;
int readVal = 0;
float V = 0;
int led = 8;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(readPin,INPUT);
  pinMode(led,OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  readVal = analogRead(readPin);
  V = (5./1023.) * readVal;
  Serial.print(readVal);
  Serial.print(" ");
  Serial.print(V);
  Serial.println("V");

  if (V < 2.5){
    digitalWrite(led,LOW);
  } else if (V<4.0){
    digitalWrite(led,HIGH);
  }
  else {
    digitalWrite(led,LOW);
  }
  
  delay(500);
}
