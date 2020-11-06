int led = 9;
int tic = 100;
int tac = 500;
int taa = 2000;
void setup() {
  // put your setup code here, to run once:
pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led,HIGH);
  delay(tic);
  digitalWrite(led,LOW);
  delay(tic);

  digitalWrite(led,HIGH);
  delay(tic);
  digitalWrite(led,LOW);
  delay(tic);

  digitalWrite(led,HIGH);
  delay(tic);
  digitalWrite(led,LOW);
  delay(tic);

  delay(1000);

  digitalWrite(led,HIGH);
  delay(tac);
  digitalWrite(led,LOW);
  delay(tac);

  digitalWrite(led,HIGH);
  delay(tac);
  digitalWrite(led,LOW);
  delay(tac);

  digitalWrite(led,HIGH);
  delay(tac);
  digitalWrite(led,LOW);
  delay(tac);

  delay(1000);

  digitalWrite(led,HIGH);
  delay(taa);
  digitalWrite(led,LOW);
  delay(10);

  delay(1000);
}
