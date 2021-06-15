#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <AccelStepper.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
#define EN1_PIN 8
//driver for the axis 2 - Y
#define PUL2_PIN 4
#define DIR2_PIN 7

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);

#define RUN A3 //start in home
#define DOWN A0
#define MODE A1
#define UP A2

#define STATE_STARTUP 0
#define STATE_HOMEX1 4
#define STATE_HOMEX2 5
#define STATE_HOMEZ1 6
#define STATE_HOMEZ2 7

LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

byte currentState = STATE_STARTUP; //STATE_WAITSTART;//


int microStep[2] = {4, 1};
float angleStep[2] = {1.8, 1.8};
float disPerRound[2] = {40, 360};

long maxSpeeds[2] = {50000, 2000};
long acceleration[2] = {40000, 1500};

float setPositions[4] = {0,  0, 0, 180};
long setSteps[4] = {0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);

  digitalWrite(EN1_PIN, LOW);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Cap phoi tu dong");
  delay(3000);
  lcd.clear();
}

void loop() {
  setLCD();
  updateState(currentState);
}
void updateState(byte aState) {
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      currentState = STATE_HOMEX1;
      break;
    case STATE_HOMEX1:
      Serial.println("STATE_HOMEX1");
      setPositions[0] = ajustValue(currentState);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('0');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEX2;
        lcd.clear();
      }
      break;
    case STATE_HOMEX2:
      Serial.println("STATE_HOMEX2");
      setPositions[1] = ajustValue(currentState);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('1');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEZ1;
        lcd.clear();
      }
      break;

    case STATE_HOMEZ1:
      Serial.println("STATE_HOMEZ1");
      setPositions[2] = ajustValue(currentState);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('2');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEZ2;
        lcd.clear();
      }
      break;

    case STATE_HOMEZ2:
      Serial.println("STATE_HOMEZ2");
      setPositions[3] = ajustValue(currentState);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('3');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_STARTUP;
        lcd.clear();
      }
      break;
  }
}

float ajustValue(byte mode) {
  int count = 0;
  if (analogRead(UP) > 500) {
    count++;
    while (analogRead(UP) > 500);
  } else if (analogRead(DOWN) > 500) {
    count--;
    while (analogRead(DOWN) > 500);
  }
  switch (mode)
  {
    case STATE_HOMEX1:
      return setPositions[0] + count;
      break;
    case STATE_HOMEX2:
      return setPositions[1] + count;
      break;
    case STATE_HOMEZ1:
      return setPositions[2] + count;
      break;
    case STATE_HOMEZ2:
      return setPositions[3] + count;
      break;
  }
}


void moveOne(char m) {
  setSteps[0] = setPositions[0] * 360 / angleStep[0] * microStep[0] / disPerRound[0];
  setSteps[1] = setPositions[1] * 360 / angleStep[1] * microStep[0] / disPerRound[0];
  setSteps[2] = setPositions[2] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  setSteps[3] = setPositions[3] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  stepper1.setMaxSpeed(maxSpeeds[0]);
  stepper1.setAcceleration(acceleration[0]);
  stepper2.setMaxSpeed(maxSpeeds[1]);
  stepper2.setAcceleration(acceleration[1]);
  if (m == '0') {
    stepper1.runToNewPosition(setSteps[0]);
  } else if (m == '1') {
    stepper1.runToNewPosition(setSteps[1]);
  } else if (m == '2') {
    stepper2.runToNewPosition(setSteps[2]);
  } else if (m == '3') {
    stepper2.runToNewPosition(setSteps[3]);
  }
}

void setLCD() {
  char f1 = ':';
  char f2 = ':';
  char f3 = ':';
  char f4 = ':';

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_HOMEX1:
        f1 = ' ';
        break;
      case STATE_HOMEX2:
        f2 = ' ';
        break;
      case STATE_HOMEZ1:
        f3 = ' ';
        break;
      case STATE_HOMEZ2:
        f4 = ' ';
        break;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("X1");
  lcd.print(f1);
  lcd.print(setPositions[0], 0);
  lcd.print("  ");

  lcd.setCursor(7, 0);
  lcd.print("X2");
  lcd.print(f2);
  lcd.print(setPositions[1], 0);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print("Z1");
  lcd.print(f3);
  lcd.print(setPositions[2], 0);

  lcd.setCursor(7, 1);
  lcd.print("Z2");
  lcd.print(f4);
  lcd.print(setPositions[3], 0);

}
