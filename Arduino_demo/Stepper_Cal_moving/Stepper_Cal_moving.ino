// Control stepper speed from an Arduino UNO.
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;

long pulsePerRoundX = 200; // so xung/vong
long distancePerRoundX = 4 ; //mm
long pulsePerDistanceX= pulsePerRoundX/distancePerRoundX;  //pulse/mm

void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir

  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, LOW);
}

void loop() {
  moveX(50, LOW);
  delay(2000);
  moveX(50, HIGH);
  delay(2000);
}

//moving dx (mm);
void moveX(long dx, bool dir) {
  long stepsCount = 0;
  long totalPulse = dx*pulsePerDistanceX;
  
  Serial.print("Move ");
  Serial.print(dx);
  Serial.println(" mm ");

  digitalWrite(dirPinX, dir);
  while (stepsCount < totalPulse) {
    stepsCount++;
    digitalWrite(stepPinX, HIGH);
    delay(1);
    digitalWrite(stepPinX, LOW);
    delay(5);
  }
}
