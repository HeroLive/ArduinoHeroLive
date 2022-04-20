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

  
  //trigger on Digital D0
  if (flameD0 == 1) {
    digitalWrite(buzzer, HIGH);
    delay(2000);
  } else {
    digitalWrite(buzzer,LOW);
  }
  Serial.println(flameD0);
  /*
  //trigger on Analog A0
  if (flameA0 < 400) {
    digitalWrite(buzzer, HIGH);
//    delay(2000);
  } else {
    digitalWrite(buzzer,LOW);
  }
  Serial.println(flameA0);
  */
  delay(20);
}
