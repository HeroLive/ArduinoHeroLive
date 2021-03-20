// Control stepper speed from an Arduino UNO.
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;

int speedPinX = A3;
long totalRoundX = 10;
long pulsePerRoundX = 200; // so xung/vong
float speedX = 60  ; // vong/phut

void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(speedPinX, INPUT);

  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, LOW);
}

void loop() {
  speedX = map(analogRead(speedPinX), 0, 1023, 20, 240); //vong/phut
  //  Serial.println(speedX);
  moveX(2, LOW, speedX);
  delay(2000);
}

void moveX(long totalRound, bool dir, float speed_) {
  long t_time = millis();
  long stepsCount = 0;
  float period =   1000000 / (speed_ / 60 * pulsePerRoundX); // time micro second
  long totalPulse = totalRound * pulsePerRoundX;
  
  Serial.print("Total Round X is ");
  Serial.print(totalRound);
  Serial.print(" with speed ");
  Serial.print(speed_);
  Serial.println(" round/min");

  digitalWrite(dirPinX, dir);
  while (stepsCount < totalPulse) {
    stepsCount++;
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(0.2 * period);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(0.8 * period);
  }
}
