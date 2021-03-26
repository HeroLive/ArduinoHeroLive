int pinA0 = A0;
int pinD0 = 2;
int buzzer = 3;
int flameA0;
int flameD0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinA0, INPUT);
  pinMode(pinD0, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  flameA0 = analogRead(pinA0);
  flameD0 = digitalRead(pinD0);
  if (flameA0 < 800) {
    digitalWrite(buzzer, HIGH);
    delay(2000);
  } else {
    digitalWrite(buzzer,LOW);
  }
  Serial.println(flameA0);
  Serial.print(" ");
  //Serial.println(flameD0);
  delay(20);
}
