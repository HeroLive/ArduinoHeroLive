// Control stepper speed from an Arduino UNO.
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;

long pulsePerRoundX = 400; // so xung/vong
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
  delay(2000);
  moveX(10, LOW);  //move 10mm
  delay(1000);
  moveX(10, LOW);  //move +10mm  (~20 mm)
  delay(1000);
  moveX(20, HIGH); // move to Zero
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
