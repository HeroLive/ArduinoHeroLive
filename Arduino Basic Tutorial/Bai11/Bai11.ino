int numBlink;
int led = 2;
int dt = 500;
String msg01 = "How many Blinks do you want:";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(msg01);
  while(Serial.available()==0){
    
  }
  numBlink = Serial.parseInt();
  Serial.println(numBlink);
  for(int i=1;i<=numBlink;i=i+1){
    digitalWrite(led,HIGH);
    delay(dt);
    digitalWrite(led,LOW);
    delay(dt);
  }
  
}
