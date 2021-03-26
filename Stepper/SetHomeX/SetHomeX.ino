int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;
int endPinXLeft = 9;
int endPinXRight = 11;

#define STATE_STARTUP 0
#define STATE_SETTING 1
#define STATE_GO_HOME 2
#define STATE_MOVE 3

byte currentState = STATE_STARTUP;

long pulsePerRound = 200; // so xung/vong
long period =   10000; // time bettween 2 pulse - us

// ============== defaut value ===================
float speedScroll = 60  ; // vong/phut
bool dirXDefaut = LOW;
//================================================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(endPinXLeft, INPUT_PULLUP); // end stop left
  pinMode(endPinXRight, INPUT_PULLUP); // end stop right

  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, dirXDefaut);

  updateState(STATE_SETTING);
}

void loop() {
  // put your main code here, to run repeatedly:
}

//------------------------------------------------------------------------------------
void updateState(byte aState)
{
  if (aState == currentState)
  {
    return;
  }

  // do state change
  switch (aState)
  {
    case STATE_SETTING:
      delay(1000);
      Serial.println("STATE_SETTING");
      setting();
      break;
    case STATE_GO_HOME:
      delay(1000);
      Serial.println("STATE_GO_HOME");
      goHomeX();
      break;
    case STATE_MOVE:
      delay(1000);
      Serial.println("STATE_MOVE");
      moving();
      break;
  }
  currentState = aState;
}
// ----------------------------------------------
void setting() {
  updateState(STATE_GO_HOME);
}
// ----------------------------------------------
void goHomeX() {
  speedScroll = 10; //vong/phut
  period = 1000000 / (speedScroll / 60 * pulsePerRound);
  Serial.println(period);
  digitalWrite(dirPinX, !dirXDefaut); //go to left HIGH
  while (true) {
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(0.5 * period);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(0.5 * period);
    if (digitalRead(endPinXLeft) == 0) {
      digitalWrite(dirPinX, dirXDefaut);
      updateState(STATE_MOVE);
    }
  }
}
// -----------------------------------------------
void moving() {
  speedScroll = 30;
  period = 1000000 / (speedScroll / 60 * pulsePerRound);
  Serial.println("Moving ...");
  while (true) {
    if (digitalRead(endPinXLeft) == 0) {
      digitalWrite(dirPinX, dirXDefaut);
    }
    if (digitalRead(endPinXRight) == 0) {
      digitalWrite(dirPinX, !dirXDefaut);
    }

    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(0.5 * period);

    digitalWrite(stepPinX, LOW);
    delayMicroseconds(0.5 * period);
  }
}
