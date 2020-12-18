// Run a A4998 Stepstick from an Arduino UNO.
int x = 0;
int n = 1;
int en = 8 ;
int dirX = 5 ;
int stepX = 2 ;
int limitX = 9;
int state_sw_X = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepX, OUTPUT); // Step
  pinMode(dirX, OUTPUT); // Dir
  pinMode(limitX, INPUT_PULLUP); // Limit X
  digitalWrite(en, LOW); // Set Enable low
  do {
    state_sw_X = digitalRead(limitX);
    digitalWrite(dirX, LOW); //set direction to move x=0
    Serial.println(state_sw_X);
    digitalWrite(stepX, HIGH);
    delay(10);
    digitalWrite(stepX, LOW);
    delay(10);
  } while (state_sw_X == 1);
  delay(2000);
}

void loop() {
  x = n * 200;
  digitalWrite(dirX, HIGH);
  for (int i = 1 ; i <= x ; i = i + 1) {
    digitalWrite(stepX, HIGH);
    delay(5);
    digitalWrite(stepX, LOW);
    delay(5);
  }
  delay(2000);
  digitalWrite(dirX, LOW);
  for (int i = 1 ; i <= x ; i = i + 1) {
    digitalWrite(stepX, HIGH);
    delay(1);
    digitalWrite(stepX, LOW);
    delay(1);
  }
  delay(2000);

}
