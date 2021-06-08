#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

//driver for the axis 1 - X
#define PUL1_PIN 5
#define DIR1_PIN 6
#define EN1_PIN 7
//driver for the axis 2 - Y
#define PUL2_PIN 9
#define DIR2_PIN 10
#define EN2_PIN 11
#define STOP A0
#define DOWN A1
#define MODE A2
#define UP A3
boolean DIR2 = LOW;

#define STATE_STARTUP 0
#define STATE_DIAMETER 2
#define STATE_LENGTH 3
#define STATE_SPEED 4
#define STATE_MANUALMOVING 5
#define STATE_WAITMOVE 6
#define STATE_MOVING 7

LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

byte currentState = STATE_STARTUP;
unsigned long LastStateChangeTime;
char step2_side = 'L';

long steps[2] = {0, 0};
int microStep[2] = {16, 16};
float angleStep[2] = {1.8, 1.8};
float disPerRound = 0.125; // round/mm
float fullSpeed = microStep[0] * 1000;

float diameter = 0.5; //mm
float L_rolling = 10 ; //mm
float PercentSpeed = 10; //%
float N_rolling = L_rolling / diameter;
float positions[2] = {0, -L_rolling};  //X: rolling(round), Y: left-right (mm)
float N_display = 0;
float curSpeed = fullSpeed * PercentSpeed / 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  pinMode(EN2_PIN, OUTPUT);
  digitalWrite(EN1_PIN, LOW);
  digitalWrite(EN2_PIN, LOW);
  digitalWrite(DIR1_PIN, LOW); //reverse clock
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("  May quan day  ");
  //  delay(3000);
  lcd.clear();

}

void loop() {
  //  delay(200);
  //  Serial.print(analogRead(STOP));
  //  Serial.print(" ");
  //  Serial.print(analogRead(DOWN));
  //  Serial.print(" ");
  //  Serial.print(analogRead(MODE));
  //  Serial.print(" ");
  //  Serial.println(analogRead(UP));
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
      //      Serial.println("STATE_STARTUP");
      //      currentState = STATE_LENGTH;
      digitalWrite(EN1_PIN, HIGH);
      digitalWrite(EN2_PIN, HIGH);
      if (analogRead(MODE) > 500) {
        currentState = STATE_LENGTH;
        while (analogRead(MODE) > 500);
        digitalWrite(EN1_PIN, LOW);
        digitalWrite(EN2_PIN, LOW);
        lcd.clear();
      }
      break;
    case STATE_LENGTH:
      //      Serial.println("STATE_LENGTH");
      L_rolling = ajustValue(STATE_LENGTH);
      if (analogRead(MODE) > 500) {
        currentState = STATE_DIAMETER;
        while (analogRead(MODE) > 500);
      }
      break;
    case STATE_DIAMETER:
      //      Serial.println("STATE_DIAMETER");
      diameter = ajustValue(STATE_DIAMETER);
      if (analogRead(MODE) > 500) {
        currentState = STATE_SPEED;
        N_rolling = L_rolling / diameter;
        while (analogRead(MODE) > 500);
      }
      break;
    case STATE_SPEED:
      //      Serial.println("STATE_SPEED");
      PercentSpeed = ajustValue(STATE_SPEED);
      if (analogRead(MODE) > 500) {
        currentState = STATE_MANUALMOVING;
        while (analogRead(MODE) > 500);
        curSpeed = map(PercentSpeed, 10, 100, 200, 32);
        Serial.println(curSpeed);
      }
      break;
    case STATE_MANUALMOVING:
      Serial.println("STATE_MANUALMOVING");
      manualControl();
      break;
    case STATE_WAITMOVE:
      //      Serial.println("STATE_WAITMOVE");
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
        positions[1] = -L_rolling;
        N_display = 0;
        step2_side = 'L';
      }
      break;
    case STATE_MOVING:
      Serial.println("STATE_MOVING");
      if (positions[1] < 0) {
        positions[1] = L_rolling;
        step2_side = 'R';
        DIR2 = LOW;
      } else {
        positions[1] = -L_rolling;
        step2_side = 'L';
        DIR2 = HIGH;
      }
      //      positions[0] = positions[0] + N_rolling;
      positions[0] = N_rolling;
      runSteppers();
      currentState = STATE_WAITMOVE;
      lcd.clear();
      break;
  }
  //  currentState = aState;
}
void move12(long nStep[2]) {
  float nStepMax = nStep[0];
  float nStepMin = nStep[1];
  float ratio_max_min = nStepMax / nStepMin;
  long cnt = 0;
  float curCnt = 0;
  for (long i = 1 ; i <= nStepMax ; i = i + 1) {
    curCnt = i / (nStepMax / nStepMin);
    if (curCnt - cnt >= 1) {
      digitalWrite(PUL2_PIN, HIGH);
      cnt++;
    }

    digitalWrite(PUL1_PIN, HIGH);
    delayMicroseconds(curSpeed);

    digitalWrite(PUL1_PIN, LOW);
    digitalWrite(PUL2_PIN, LOW);
    delayMicroseconds(curSpeed);
    if (analogRead(STOP) > 500) {
      while (analogRead(STOP) > 500);
      Serial.println("PAUSED");
      while (1) {
        if (analogRead(STOP) > 500) {
          long t = millis();
          while (analogRead(STOP) > 500);
          if (millis() - t > 2000) {
            i = nStepMax + 1;
          }
          break;
        }
      }
    }
  }
  N_display = (N_display + nStepMax / (360 * microStep[0] / angleStep[0]));
}
void runSteppers()
{
  Serial.print("Move to ");
  Serial.print(N_display + positions[0]);
  Serial.print(" ");
  Serial.println(positions[1]);
  digitalWrite(DIR2_PIN, DIR2);
  steps[0] = positions[0] * 360 * microStep[0] / angleStep[0];
  steps[1] = abs(positions[1]) * disPerRound * 360 * microStep[1] / angleStep[1];
  move12(steps);
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
      PercentSpeed = PercentSpeed + 1 * count;
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

  switch (currentState)
  {
    case STATE_STARTUP:
      m_ = 'D';
      break;
    case STATE_LENGTH:
      m_ = 'E';
      break;
    case STATE_DIAMETER:
      m_ = 'E';
      break;
    case STATE_SPEED:
      m_ = 'E';
      break;
    case STATE_MOVING:
      m_ = '#';
      break;
  }

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_STARTUP:
        m_ = ' ';
        break;
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
  lcd.print("m");
  lcd.print(' ');
  lcd.setCursor(7, 0); //frint from column 3, row 0
  lcd.print("D");
  lcd.print(d_);
  lcd.print(diameter, 2);
  lcd.print("m");

  lcd.setCursor(0, 1);
  lcd.print("S");
  lcd.print(s_);
  lcd.print(PercentSpeed, 0);
  lcd.print('%');
  lcd.print(' ');
  lcd.setCursor(7, 1);
  lcd.print("V");
  lcd.print(v_);
  lcd.print(N_display, 1);
  lcd.print("     ");
  lcd.setCursor(15, 1);
  lcd.print(m_);
}
void manualControl() {
  int m = 1;
  updateMLCD(0, ' ');
  while (1) {
    if (analogRead(MODE) > 500) {
      currentState = STATE_WAITMOVE;
      lcd.clear();
      while (analogRead(MODE) > 500);
      break;
    }
    if (analogRead(STOP) > 500) {
      while (analogRead(STOP) > 500);
      if (m == 1) {
        m = 2;
      } else m = 1;
      updateMLCD(m, 'L');
    }
    if (analogRead(UP) > 500) {
      updateMLCD(m, 'R');
      while (analogRead(UP) > 500) {
        moveOne(m, 'R');
      }
    } else if (analogRead(DOWN) > 500) {
      updateMLCD(m, 'L');
      while (analogRead(DOWN) > 500) {
        moveOne(m, 'L');
      }
    }
  }
}
void moveOne(int m, char LR) {
  if (LR == 'L') {
    digitalWrite(DIR1_PIN, HIGH);
    digitalWrite(DIR2_PIN, HIGH);
  }
  if (LR == 'R') {
    digitalWrite(DIR1_PIN, LOW);
    digitalWrite(DIR2_PIN, LOW);
  }
  if (m == 1) {
    digitalWrite(PUL1_PIN, HIGH);
    delayMicroseconds(curSpeed);
    digitalWrite(PUL1_PIN, LOW);
    delayMicroseconds(curSpeed);
  }
  if (m == 2) {
    digitalWrite(PUL2_PIN, HIGH);
    delayMicroseconds(16*curSpeed);
    digitalWrite(PUL2_PIN, LOW);
    delayMicroseconds(16*curSpeed);
  }

}
void updateMLCD(int M, char LR) {
  char m1L_ = ' ';
  char m2L_ = ' ';
  char m1R_ = ' ';
  char m2R_ = ' ';
  if (M == 1) {
    if (LR == 'L') {
      m1L_ = '<';
    } else m1R_ = '>';
  }
  if (M == 2) {
    if (LR == 'L') {
      m2L_ = '<';
    } else m2R_ = '>';
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print('<');
  lcd.print(m1L_);
  lcd.setCursor(7, 0);
  lcd.print("M1");
  lcd.setCursor(14, 0);
  lcd.print(m1R_);
  lcd.print('>');

  lcd.setCursor(0, 1);
  lcd.print('<');
  lcd.print(m2L_);
  lcd.setCursor(7, 1);
  lcd.print("M2");
  lcd.setCursor(14, 1);
  lcd.print(m2R_);
  lcd.print('>');
}
