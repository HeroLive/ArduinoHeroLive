int buzzPin = 2;
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 1; i<=10 ; i=i+1){
    digitalWrite(buzzPin,HIGH);
    delay(500);
    digitalWrite(buzzPin,LOW);
    delay(500);
  }
  for (int i = 1; i<=10 ; i=i+1){
    digitalWrite(buzzPin,HIGH);
    delay(200);
    digitalWrite(buzzPin,LOW);
    delay(200);
  }

  digitalWrite(buzzPin,HIGH);
  delay(5000);
  digitalWrite(buzzPin,LOW);
  delay(5000);
  
  
}
