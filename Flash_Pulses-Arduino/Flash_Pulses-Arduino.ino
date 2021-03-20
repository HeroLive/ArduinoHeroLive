
int ledPin = 13; // set the signal pin out here.
int pulseNum = 400; // set the Pulse number here.
int pulseDuration = 50; // set the Pulse width here.
int button = A0;

unsigned long LastStateChangeTime;
unsigned long StateWaitTime;

#define STATE_STARTUP 0
#define STATE_SETTING 1
#define STATE_IDLE 2
#define STATE_PULSE 3

byte currentState = STATE_STARTUP;


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT); // Trigger switch.
  pinMode(ledPin, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(ledPin, HIGH);
  updateState(STATE_SETTING);
}

void loop()
{
  //  int x = analogRead(button);
  //  Serial.println(x);
  //  if (digitalRead(2) == LOW)
  //  {
  //    blinkyBlinky(pulseNum, pulseDuration);
  //  }
}

void pulse(int repeats, int pulseDuration)
{
  for (int i = 0; i < repeats; i++)
  {
    digitalWrite(ledPin, LOW);
    delay(pulseDuration);
    digitalWrite(ledPin, HIGH);
    delay(pulseDuration);
  }
  updateState(STATE_IDLE);
}


// ---------------------------
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
      setting();
      break;
    case STATE_IDLE:
      StateWaitTime = 1000;
      idle();
      break;
    case STATE_PULSE:
      StateWaitTime = 1000;
      Serial.println("STATE_PULSE");
      pulse(pulseNum, pulseDuration);
      break;
  }

  currentState = aState;
  LastStateChangeTime = millis();
}
// ---------------------------
void setting() {
  while (1) {
    Serial.println("STATE_SETTING");
    if (analogRead(button) > 600 && analogRead(button) < 800) {
      while (1) {
        if (analogRead(button) > 800) {
          updateState(STATE_IDLE);
          return;
        }
      }
    }
  }
}

void idle() {
  while (1) {
    Serial.println("STATE_IDLE");
    if (analogRead(button) > 600 && analogRead(button) < 800) {
      while (1) {
        if (analogRead(button) > 800) {
          updateState(STATE_SETTING);
          return;
        }
      }
    }
    if (analogRead(button) > 60 && analogRead(button) < 200) {
      updateState(STATE_PULSE);
      return;
    }
  }
}
