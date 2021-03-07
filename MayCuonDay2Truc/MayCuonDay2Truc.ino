int en = 8 ;
int dirPinA = 7 ;
int stepPinA = 4 ;
int dirPinX = 5 ;
int stepPinX = 2 ;
int speedPin = A3; //coolant enable --> change speed
int startPin = A2; //Start/resume
int pausePin = A1; //Feed hold
int stopPin = A0;

int endPinXLeft = 9;
int endPinXRight = 11;
int lastEndX = 1;


#define STATE_STARTUP 0
#define STATE_SETTING 2
#define STATE_GO_HOME 3
#define STATE_PAUSE 4
#define STATE_MOVE 5

byte currentState = STATE_STARTUP;
unsigned long LastStateChangeTime;
unsigned long StateWaitTime;

int pulseMax = 0; //print to monitor
int pulseMin = 0; //print to monitor

//input field
float ratioAX = 2;
long totalRoundA = 40;

long pulsePerRound = 200; // so xung/vong
long period =   10000; // time bettween 2 pulse - us
long stepsCount = 0;
long stepsCountX = 0;

// ============== defaut value ===================
float speedScroll = 60  ; // vong/phut
float ratioAX_defaut = 2;
long totalRoundA_defaut = 10000;
bool dirXDefaut = LOW;
//================================================

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinA, OUTPUT); // Step
  pinMode(dirPinA, OUTPUT); // Dir
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir

  pinMode(endPinXLeft, INPUT_PULLUP); // end stop left
  pinMode(endPinXRight, INPUT_PULLUP); // end stop right
  pinMode(speedPin, INPUT);
  pinMode(startPin, INPUT_PULLUP); // start
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);

  digitalWrite(dirPinA, LOW); //
  digitalWrite(dirPinX, dirXDefaut);
  updateState(STATE_SETTING);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// ------------------------------------------------------
void setting() {
  Serial.println("Setting ...");
  //from input
  ratioAX = ratioAX_defaut;
  totalRoundA = totalRoundA_defaut;

  while (true) {
    //        Serial.println(digitalRead(startPin));
    speedScroll = map(analogRead(speedPin), 0, 1023, 20, 120);
    period =   1000000 / (speedScroll / 60 * pulsePerRound); // time bettween 2 pulse
//    Serial.println(period);
    if (digitalRead(startPin) == 0) {
//      Serial.print("Total Round A ");
//      Serial.print(totalRoundA);
//      Serial.print(" with speed ");
//      Serial.print(speedScroll);
//      Serial.println(" round/min");
      stepsCount = 0;
      stepsCountX = 0;
      updateState(STATE_GO_HOME);
    }
  }
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
    case STATE_STARTUP:
      StateWaitTime = 1000;
      Serial.println("STATE_STARTUP");
      break;
    case STATE_SETTING:
      StateWaitTime = 1000;
      Serial.println("STATE_SETTING");
      setting();
      break;
    case STATE_GO_HOME:
      StateWaitTime = 1000;
      Serial.println("STATE_GO_HOME");
      goHomeX();
      break;
    case STATE_PAUSE:
      StateWaitTime = 1000;
      Serial.println("STATE_PAUSING");
      pause();
      break;
    case STATE_MOVE:
      StateWaitTime = 1000;
      Serial.println("STATE_MOVE");
      moving();
      break;
  }

  currentState = aState;
  LastStateChangeTime = millis();
}
// ----------------------------------------------
void goHomeX() {
  speedScroll = 20;
  period = 1000000 / (speedScroll / 60 * pulsePerRound);
  long period_go_home = period * ratioAX;
  Serial.println(period_go_home);
  digitalWrite(dirPinX, !dirXDefaut); //go to left
  while (true) {
//    Serial.println(period);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(period_go_home);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(period_go_home);
    if (digitalRead(endPinXLeft) == 0) {
      digitalWrite(dirPinX, dirXDefaut);
      updateState(STATE_MOVE);
    }
    if (digitalRead(stopPin) == 0) {
      updateState(STATE_SETTING);
    }
  }
}
//-----------------------------------------------
void pause() {
  Serial.println("Pausing ...");
  while (true) {
    speedScroll = map(analogRead(speedPin), 0, 1023, 20, 120);
    period = 1000000 / (speedScroll / 60 * pulsePerRound);
//    Serial.println(period);
    if (digitalRead(startPin) == 0) {
      updateState(STATE_MOVE);
    }
    if (digitalRead(stopPin) == 0) {
      updateState(STATE_SETTING);
    }
  }
}
// -----------------------------------------------
void moving() {
  //default step X min, step A max
  long t_time = millis();
  long currentStepsCountX = 0;
  
  long totalPulse = totalRoundA * pulsePerRound;
  Serial.println("Moving ...");
//  Serial.println(stepsCount);
  while (stepsCount <= totalPulse) {
//    Serial.println(period);
    if (digitalRead(stopPin) == 0) {
      updateState(STATE_SETTING);
    }
    if (digitalRead(pausePin) == 0) {
      updateState(STATE_PAUSE);
    }

    if (millis() - t_time >= 1000) {
      t_time = millis();
      speedScroll = map(analogRead(speedPin), 0, 1023, 20, 120);
      period = 1000000 / (speedScroll / 60 * pulsePerRound);
    }

    if (digitalRead(endPinXLeft) == 0) {
      digitalWrite(dirPinX, dirXDefaut);
    }
    if (digitalRead(endPinXRight) == 0) {
      digitalWrite(dirPinX, !dirXDefaut);
    }

    stepsCount++;
    currentStepsCountX = stepsCount / ratioAX;

    if (currentStepsCountX - stepsCountX >= 1) {
      digitalWrite(stepPinX, HIGH);
      pulseMin = 1;
      stepsCountX++;
    }
    digitalWrite(stepPinA, HIGH);
    delayMicroseconds(0.2 * period);
//
//        pulseMax = 1;
//        Serial.print(pulseMax);
//        Serial.print(" ");
//        Serial.println(pulseMin);

    digitalWrite(stepPinA, LOW);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(0.8 * period);

//        pulseMax = 0;
//        pulseMin = 0;
//        Serial.print(pulseMax);
//        Serial.print(" ");
//        Serial.println(pulseMin);

  }
  Serial.println("Moved to target");
//  Serial.println(stepsCount);
  delay(1000);
  updateState(STATE_SETTING);
}
