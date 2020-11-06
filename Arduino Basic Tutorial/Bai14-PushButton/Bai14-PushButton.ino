int btPin = 2;
int btState;
int led =3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(btPin,INPUT_PULLUP);
  pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  btState = digitalRead(btPin);
  if(btState==0){
    digitalWrite(led,HIGH);
  } else {
    digitalWrite(led,LOW);
  }
  Serial.println(btState);
  delay(200);
}
