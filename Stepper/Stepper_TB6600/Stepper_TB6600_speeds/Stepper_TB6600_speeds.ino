/*
  // Define stepper motor connections:
  #define dirPin 2
  #define stepPin 3
  void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // Set the spinning direction CW/CCW:
  digitalWrite(dirPin, LOW);  //LOW: unclock, HIGH: clock
  }
  void loop() {
  // These four lines result in 1 step:
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
  }
*/






// Define stepper motor connections:
#define stepPin 2
#define dirPin 5
#define en 8

int microStep = 8; //200 xung/vong
float angleStep = 1.8;
float stepsPerRound = microStep * 360.0 / angleStep; //200 xung
long steps = 1 * stepsPerRound;
int speedV = 240; //rpm
float speeds = 1.0 / (speedV/60 * stepsPerRound)  / 2.0 * 1000000;
void setup() {
  Serial.begin(9600);
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(en, OUTPUT);

  digitalWrite(en, LOW);
  delay(5000);
  Serial.println(speeds);
}
void loop() {

  delay(1000);

  // 2 round in clock
  digitalWrite(dirPin, HIGH);
  steps = 15 * stepsPerRound; //400
  Serial.print("Move to ");
  Serial.print(steps / stepsPerRound, 0);
  Serial.print(" rounds ");
  Serial.println(steps);
  for (long i = 1; i <= steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speeds);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speeds);
  }

  delay(1000);

  // 5 round in unclock
  digitalWrite(dirPin, LOW);
  steps = 15 * stepsPerRound;
  Serial.print("Move to ");
  Serial.print(steps / stepsPerRound, 0);
  Serial.print(" rounds ");
  Serial.println(steps);
  for (long i = 1; i <= steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speeds);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speeds);
  }
}
