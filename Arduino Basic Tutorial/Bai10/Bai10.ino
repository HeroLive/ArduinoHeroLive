//while
int led = 2;
int readPin = A0;
int readVal=0;
int i=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(readPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readVal = analogRead(readPin);
  Serial.println(readVal);
  delay(100);
  
  while(readVal>500){
    digitalWrite(led,HIGH);
    readVal = analogRead(readPin);
    Serial.println(readVal);
    delay(100);
  }
  digitalWrite(led,LOW);
}
