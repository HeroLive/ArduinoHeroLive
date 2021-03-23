// Control stepper speed from an Arduino UNO.
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;

#define STOP 0
#define CLOCKWISE 1
#define ANTI_CLOCKWISE 2

int dir_current_status = 0;
int dir_last_status = 0;
bool dirX = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir

  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, dirX);
}


void loop() {
  // put your main code here, to run repeatedly:
  dir_current_status = dirStatus();
  switch (dir_current_status) {
    case STOP:
      Serial.println("STOP");
      break;
    case CLOCKWISE:
      Serial.println("CLOCKWISE");
      digitalWrite(dirPinX, dirX);
      break;
    case ANTI_CLOCKWISE:
      Serial.println("ANTI_CLOCKWISE");
      digitalWrite(dirPinX, !dirX);
      break;
  }
  digitalWrite(stepPinX, HIGH);
  delay(1);
  digitalWrite(stepPinX, LOW);
  delay(5);
  dir_last_status = dir_current_status;
}

int dirStatus() {
  int value = analogRead(A0);
  if (value < 100) {
    return CLOCKWISE;
  } else if (value < 900) {
    return STOP;
  } else {
    return ANTI_CLOCKWISE;
  }
}
