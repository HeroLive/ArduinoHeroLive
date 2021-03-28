// Control stepper speed from an Arduino UNO.

//X
int enX = 38 ;
int dirPinX = A1 ;
int stepPinX = A0 ;
//Y
int enY = A2 ;
int dirPinY = A7 ;
int stepPinY = A6 ;
//Z
int enZ = A8 ;
int dirPinZ = 48 ;
int stepPinZ = 46 ;
//E0
int enE0 = 24 ;
int dirPinE0 = 28 ;
int stepPinE0 = 26 ;
//E1
int enE1 = 30 ;
int dirPinE1 = 34 ;
int stepPinE1 = 36 ;

long pulsePerRound = 200; // so xung/vong

void setup()
{
  Serial.begin(9600);
  pinMode(enX, OUTPUT); // Enable
  pinMode(enY, OUTPUT);
  pinMode(enZ, OUTPUT);
  pinMode(enE0, OUTPUT);
  pinMode(enE1, OUTPUT);
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(stepPinY, OUTPUT);
  pinMode(stepPinZ, OUTPUT);
  pinMode(stepPinE0, OUTPUT);
  pinMode(stepPinE1, OUTPUT);
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(dirPinY, OUTPUT);
  pinMode(dirPinZ, OUTPUT);
  pinMode(dirPinE0, OUTPUT);
  pinMode(dirPinE1, OUTPUT);

  digitalWrite(enX, LOW); // Set Enable low
  digitalWrite(enY, LOW);
  digitalWrite(enZ, LOW);
  digitalWrite(enE0, LOW);
  digitalWrite(enE1, LOW);
}

void loop() {
  moving(10, stepPinX, dirPinX, LOW); // move 10 round
  delay(200);
  moving(5, stepPinY, dirPinY, LOW);  //move 10 round
  delay(100);
  moving(5, stepPinZ, dirPinZ, LOW);  //move 10 round
  delay(100);
  moving(5, stepPinE0, dirPinE0, LOW);  //move 10 round
  delay(100);
  moving(5, stepPinE1, dirPinE1, LOW);  //move 10 round
  delay(100);

}

//moving n round;
void moving(int n, int stepPin, int dirPin, bool dir) {
  long stepsCount = 0;
  long totalPulse = n * pulsePerRound;

  digitalWrite(dirPin, dir);
  while (stepsCount < totalPulse) {
    stepsCount++;
    digitalWrite(stepPin, HIGH);
    delay(1);
    digitalWrite(stepPin, LOW);
    delay(5);
  }
}
