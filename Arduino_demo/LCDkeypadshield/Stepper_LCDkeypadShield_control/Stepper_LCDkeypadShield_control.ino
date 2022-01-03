#include <LiquidCrystal.h>
#include <StepperMotor.h>
//LCD pin to Arduino
#define pin_RS 8
#define pin_EN 9
#define pin_d4 4
#define pin_d5 5
#define pin_d6 6
#define pin_d7 7
#define pin_BL 10
LiquidCrystal lcd( pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
// define some values used by the panel and buttons
int lcd_key = 0;
int last_lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 5
#define btnUP 3
#define btnDOWN 4
#define btnLEFT 2
#define btnSELECT 1
#define btnNONE 0

//stepper motor
#define PUL1_PIN 11
#define DIR1_PIN 12
#define EN1_PIN 13
StepperMotor stepper(PUL1_PIN, DIR1_PIN);

float disPerRound = 1.0 ;// only round: 1.0 round, vitme T8-4mm: 4.0 mm, GT2 Pulley 16 Teeth: 16x2 = 32.0 mm
int microStep = 2; //1: full step, 2: haft step, ...
float angleStep = 1.8; //a step angle of 1.8 degrees with 200 steps per revolution
float stepsPerUnit = (1 / disPerRound) * 360.0 * microStep / angleStep; //steps/round or steps/mm ...
float speed = 2.0; //set 2 round/s

boolean motorType = false; //0: stop, 1: running
float target = 0;
String status = "DisEna";

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  updateLCD();

  pinMode(EN1_PIN, OUTPUT);
  digitalWrite(EN1_PIN, HIGH);
  delay(5000);
  digitalWrite(EN1_PIN, LOW);
  stepper.setStepsPerUnit(stepsPerUnit);
  stepper.setSpeed(speed);
  stepper.setStartDirection(HIGH);
  status = "Set   ";
  updateLCD();
}

void loop() {
  if (motorType == 0) {
    updateSetting();
  } else if (motorType == 1) {
    status = "Run   ";
    updateLCD();
    Serial.println("Run to target");
    motorType = false;
    stepper.setSpeed(speed);
    stepper.moveTo(target);
    Serial.println("On target");
    status = "Set   ";
    updateLCD();
  }
}

void updateSetting() {
  lcd_key = read_LCD_buttons();
  switch (lcd_key) {
    case btnRIGHT: {
        if (last_lcd_key == btnNONE) {
          last_lcd_key = lcd_key;
          target = target + 0.1;
          updateLCD();
        }
        break;
      }
    case btnLEFT: {
        if (last_lcd_key == btnNONE) {
          last_lcd_key = lcd_key;
          target = target - 0.1;
          updateLCD();
        }
        break;
      }
    case btnUP: {
        if (last_lcd_key == btnNONE) {
          last_lcd_key = lcd_key;
          speed = speed + 0.1;
          updateLCD();
        }
        break;
      }
    case btnDOWN: {
        if (last_lcd_key == btnNONE) {
          last_lcd_key = lcd_key;
          speed = speed - 0.1;
          if (speed <= 0) {
            speed = 0.1;
          }
          updateLCD();
        }
        break;
      }
    case btnSELECT: {
        if (last_lcd_key == btnNONE) {
          motorType = !motorType;
          updateLCD();
        }
        break;
      }
  }
  last_lcd_key = lcd_key;
}

int read_LCD_buttons() {
  adc_key_in = analogRead(0); //A0
  if (adc_key_in < 60) {
    return btnRIGHT;
  }
  else if (adc_key_in < 200) {
    return btnUP;
  }
  else if (adc_key_in < 400) {
    return btnDOWN;
  }
  else if (adc_key_in < 600) {
    return btnLEFT;
  }
  else if (adc_key_in < 800) {
    return btnSELECT;
  } else {
    return btnNONE;
  }
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Tdo");
  lcd.setCursor(5, 0);
  lcd.print("Vtri");
  lcd.setCursor(10, 0);
  lcd.print("Tthai");
  lcd.setCursor(0, 1);
  lcd.print(speed, 1);
  lcd.setCursor(5, 1);
  lcd.print(target, 1);
  lcd.print(" ");
  lcd.setCursor(10, 1);
  lcd.print(status);
}
