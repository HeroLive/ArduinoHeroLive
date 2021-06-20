#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <AccelStepper.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
#define EN1_PIN 8

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

#define DOWN 9
#define MODE 10
#define UP 11
#define RUN 12

#define STATE_STARTUP 0
#define STATE_SPEED 1
#define STATE_HOMEX 2

byte currentState = STATE_STARTUP;

int microStep = 16;
float angleStep = 1.8;
float distancePerRound = 40; //mm
float stepsPerDistance = microStep * (360.0 / angleStep) / distancePerRound;

int dx = 0;//mm
int curX = 0;
long steps = 0;
long maxSpeeds = 3200;

String status_ = " ";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PUL1_PIN , OUTPUT);
  pinMode(DIR1_PIN , OUTPUT);
  pinMode(EN1_PIN , OUTPUT);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(RUN, INPUT_PULLUP);

  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Linear control");
  delay(1000);
  lcd.clear();
  Serial.println("STATE_STARTUP");
}

void loop() {
  // put your main code here, to run repeatedly:

  setLCD();
  updateState(currentState);  
}

void updateState(byte aState) {
  switch (aState)
  {
    case STATE_STARTUP:      
      digitalWrite(EN1_PIN, HIGH);
      status_ = "RESET";
      if (digitalRead(DOWN) == 0) {
        while (digitalRead(DOWN) == 0);
        moveOne('0');
        lcd.clear();
      }
      if (digitalRead(MODE) == 0) {
        while (digitalRead(MODE) == 0);
        currentState = STATE_HOMEX;
        digitalWrite(EN1_PIN, LOW);
        status_ = "    ";
        Serial.println("STATE_HOMEX");
        lcd.clear();
      }
      break;

    case STATE_HOMEX:
      status_ = " WAIT";
      dx = ajustValue(currentState);
      if (digitalRead(MODE) == 0) {
        while (digitalRead(MODE) == 0);
        currentState = STATE_SPEED;
        Serial.println("STATE_SPEED");
        lcd.clear();
      }
      if (digitalRead(RUN) == 0) {
        while (digitalRead(RUN) == 0);
        status_ = "  RUN";
        setLCD();
        moveOne('1');
      }
      break;

    case STATE_SPEED:
      status_ = " WAIT";
      maxSpeeds = ajustValue(currentState);
      if (digitalRead(MODE) == 0) {
        while (digitalRead(MODE) == 0);
        currentState = STATE_STARTUP;
        Serial.println("STATE_STARTUP");
        lcd.clear();
      }
      if (digitalRead(RUN) == 0) {
        while (digitalRead(RUN) == 0);
        status_ = "  RUN";
        setLCD();
        moveOne('1');
      }
      break;
  }
}

void setLCD() {
  char f1 = ':';
  char f2 = ':';

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_HOMEX:
        f1 = ' ';
        break;
      case STATE_SPEED:
        f2 = ' ';
        break;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("X");
  lcd.print(f1);
  lcd.print(dx);
  lcd.print("  ");
  lcd.setCursor(8, 0);
  lcd.print("X=");
  lcd.print(curX);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("S");
  lcd.print(f2);
  lcd.print(maxSpeeds);
  lcd.print("   ");
  lcd.setCursor(11, 1);
  lcd.print(status_);
}

void moveOne(char m) {
  
  steps = dx * stepsPerDistance;
  stepper1.setMaxSpeed(maxSpeeds);
  stepper1.setAcceleration(0.5 * maxSpeeds);
  Serial.print("Move to ");
  Serial.print(" ");
  Serial.print(dx);
  Serial.print(" ");
  Serial.print(steps);
  Serial.print(" ");
  Serial.println(maxSpeeds);
  
  if (m == '1') {
    stepper1.runToNewPosition(steps);
  }
  if (m == '0') {
    stepper1.setCurrentPosition(0);
  }
  curX = stepper1.currentPosition() / stepsPerDistance;
}


float ajustValue(byte mode) {
  int count = 0;
  long speed_ = 0;
  if (digitalRead(UP) == 0) {
    count++;
    while (digitalRead(UP) == 0);
  } else if (digitalRead(DOWN) == 0) {
    count--;
    while (digitalRead(DOWN) == 0);
  }
  switch (mode)
  {
    case STATE_HOMEX:
      return dx + 10 * count;
      break;
    case STATE_SPEED:
      speed_ = maxSpeeds + 100 * count;
      return speed_ > 0 ? speed_ : 0;
      break;
  }
}
