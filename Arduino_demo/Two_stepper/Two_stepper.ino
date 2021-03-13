// Run a A4998 Stepstick from an Arduino UNO.
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;
int dirPinZ = 7 ;
int stepPinZ = 4 ;

bool dirX = LOW;
bool dirZ = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(stepPinZ, OUTPUT); // Step
  pinMode(dirPinZ, OUTPUT); // Dir

  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, dirX);
  digitalWrite(dirPinZ, dirZ);
}

void loop() {
  moveXZ(400, stepPinX, 800, stepPinZ);
  delay(5000);
}

void moveXZ(long nStepX, int stepPinX, long nStepZ, int stepPinZ) {
  long nstep = 1;
  int X = 0;
  int Z = 0;
  while (nstep <= nStepX || nstep <= nStepZ) {
    nstep++;
    if (nstep <= nStepX) {
      digitalWrite(stepPinX, HIGH);
      X=1;
    }
    if (nstep <= nStepZ) {
      digitalWrite(stepPinZ, HIGH);
      Z=1;
    }
    Serial.print(X);
    Serial.print(" ");
    Serial.println(Z);
    delay(10);
   
    digitalWrite(stepPinX, LOW);
    digitalWrite(stepPinZ, LOW);
    X=0;
    Z=0;
    Serial.print(X);
    Serial.print(" ");
    Serial.println(Z);
    delay(10);
  }
}



/*
   X=1;
          Z=1;
    Serial.print(X);
    Serial.print(" ");
    Serial.println(Z);

    X=0;
    Z=0;
    Serial.print(X);
    Serial.print(" ");
    Serial.println(Z);

*/
