
int numLed = 13;
int delay_time = 100;
int A[] = {A0, A1, A2, A3, A4, A5, A6};
int numA = 6;
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  for (int i = 0; i <= numLed; i++) {
    pinMode(i, OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  for (int i = 2; i <= numLed; i++) {
    digitalWrite(i, HIGH);
    delay(delay_time);
  }
  for (int i = 0; i < numA; i++) {
    Serial.print(analogRead(A[i]));
    if(i==numA-1){
      Serial.println();
    } else Serial.print(" -- ");
  }
  delay(delay_time);
  
  for (int i = 2; i <= numLed; i++) {
    digitalWrite(i, LOW);
  }
  delay(delay_time);
}
