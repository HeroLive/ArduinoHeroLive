#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <AccelStepper.h>
#include <MultiStepper.h>


//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//driver for the axis 2 - Y
#define PUL2_PIN 3
#define DIR2_PIN 6
#define EN_PIN 8
#define DOWN A0  //Reset/Abort
#define MODE A1  //Feed Hold
#define UP A2   // Start/Resume

#define STATE_STARTUP 0
#define STATE_DIAMETER 2
#define STATE_LENGTH 3
#define STATE_SPEED 4
#define STATE_WAITMOVE 5
#define STATE_MOVING 6

LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F
AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);
MultiStepper steppers;

byte currentState = STATE_STARTUP;
unsigned long LastStateChangeTime;
char step2_side = 'L';

long steps[2] = {0, 0};
int microStep[2] = {16, 16};
float angleStep[2] = {1.8, 1.8};
float disPerRound = 0.5; // mm/round
float fullSpeed = microStep[0] * 500;

float diameter = 0.1; //mm
float L_rolling = 2 ; //mm
float PercentSpeed = 40; //%
float N_rolling = L_rolling / diameter;
float positions[2] = {0, 0};  //X: rolling(round), Y: left-right (mm)
long curSpeed = fullSpeed * PercentSpeed / 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  //
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("  May quan day  ");
  delay(3000);
  lcd.clear();

}

void loop() {
//  delay(200);
//  Serial.print(" : ");
//  Serial.print(positions[0]);
//  Serial.print(" ");
//  Serial.println(positions[1]);
  updateState(currentState);
  updateLCD();
}

void updateState(byte aState) {
  //  if (aState == currentState)
  //  {
  //    return;
  //  }

  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      currentState = STATE_LENGTH;
      break;
    case STATE_LENGTH:
      Serial.println("STATE_LENGTH");
      L_rolling = ajustValue(STATE_LENGTH);
      if (analogRead(MODE) > 500) {
        currentState = STATE_DIAMETER;
        while (analogRead(MODE) > 500);        
        Serial.println(N_rolling);
      }
      break;
    case STATE_DIAMETER:
      Serial.println("STATE_DIAMETER");
      diameter = ajustValue(STATE_DIAMETER);
      if (analogRead(MODE) > 500) {
        currentState = STATE_SPEED;
        N_rolling = L_rolling / diameter;
        while (analogRead(MODE) > 500);
      }
      break;
    case STATE_SPEED:
      Serial.println("STATE_SPEED");
      PercentSpeed = ajustValue(STATE_SPEED);
      if (analogRead(MODE) > 500) {
        currentState = STATE_WAITMOVE;
        while (analogRead(MODE) > 500);
        curSpeed = fullSpeed * PercentSpeed / 100;
      }
      break;
    case STATE_WAITMOVE:
      Serial.println("STATE_WAITMOVE");
      if (analogRead(MODE) > 500) {
        currentState = STATE_STARTUP;
        while (analogRead(MODE) > 500);
      }
      if (analogRead(UP) > 500) {
        while (analogRead(UP) > 500);
        currentState = STATE_MOVING;
      }
      if (analogRead(DOWN) > 500) {
        while (analogRead(DOWN) > 500);
        positions[0] = 0;
        positions[1] = 0;
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
      }
      break;
    case STATE_MOVING:
      Serial.println("STATE_MOVING");
      if (positions[1] != 0) {
        positions[1] = 0;
        step2_side = 'L';
      } else {
        positions[1] = L_rolling;
        step2_side = 'R';
      }
      positions[0] = positions[0] + N_rolling;
      runSteppers();
      currentState = STATE_WAITMOVE;
      break;
  }
  //  currentState = aState;
}

void runSteppers()
{
  Serial.print("Move to ");
  Serial.print(positions[0]);
  Serial.print(" ");
  Serial.println(positions[1]);
  stepper1.setMaxSpeed(curSpeed);
  stepper2.setMaxSpeed(curSpeed);
  steps[0] = positions[0] * 360 * microStep[0] / angleStep[0];
  steps[1] = positions[1] * disPerRound * 360 * microStep[1] / angleStep[1];
  if (EN_PIN) {
    steppers.moveTo(steps);
    steppers.runSpeedToPosition();
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
    
    case STATE_LENGTH:
      L_rolling = L_rolling + count;
      return L_rolling > 1 ? L_rolling : 1;
      break;
    case STATE_DIAMETER:
      diameter = diameter + 0.01 * count;
      return diameter > 0.1 ? diameter : 0.1;
      break;
    case STATE_SPEED:
      PercentSpeed = PercentSpeed + 10 * count;
      if (PercentSpeed < 10) {
        PercentSpeed = 10;
      } else if (PercentSpeed > 100) {
        PercentSpeed = 100;
      }
      return PercentSpeed;
      break;
  }
}

void updateLCD() {
  char d_ = ':';
  char l_ = ':';
  char s_ = ':';
  char v_ = ':';
  char m_ = step2_side;

  if (currentState == STATE_MOVING) {
    m_ = '#';
  }

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {      
      case STATE_LENGTH:
        l_ = ' ';
        break;
      case STATE_DIAMETER:
        d_ = ' ';
        break;
      case STATE_SPEED:
        s_ = ' ';
        break;
      case STATE_WAITMOVE:
        m_ = ' ';
        break;
    }
  }
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("L");
  lcd.print(l_);
  lcd.print(L_rolling, 0);
  lcd.print("mm");
  lcd.print(' ');
  lcd.setCursor(8, 0); //frint from column 3, row 0
  lcd.print("D");
  lcd.print(d_);
  lcd.print(diameter, 2);
  lcd.print("mm");

  lcd.setCursor(0, 1);
  lcd.print("S");
  lcd.print(s_);
  lcd.print(PercentSpeed, 0);
  lcd.print('%');
  lcd.print(' ');
  lcd.setCursor(8, 1);
  lcd.print("V");
  lcd.print(v_);
  lcd.print(positions[0], 0);
  lcd.print("     ");
  lcd.setCursor(15, 1);
  lcd.print(m_);
}
