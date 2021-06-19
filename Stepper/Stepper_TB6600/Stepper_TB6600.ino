
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





/*
// Define stepper motor connections:
#define dirPin 2
#define stepPin 3
int microStep = 16; //200 xung/vong
float angleStep = 1.8;
float stepsPerRound = microStep * 360.0 / angleStep; //200 xung
long steps = 1 * stepsPerRound;
void setup() {
  Serial.begin(9600);
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop() {

  delay(4000);

  // 2 round in clock
  digitalWrite(dirPin, HIGH);
  steps = 2 * stepsPerRound; //400
  Serial.print("Move to ");
  Serial.print(steps / stepsPerRound, 0);
  Serial.print(" rounds ");
  Serial.println(steps);
  for (long i = 1; i <= steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  delay(5000);

  // 5 round in unclock
  digitalWrite(dirPin, LOW);
  steps = 5 * stepsPerRound;
  Serial.print("Move to ");
  Serial.print(steps / stepsPerRound, 0);
  Serial.print(" rounds ");
  Serial.println(steps);
  for (long i = 1; i <= steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

*/
