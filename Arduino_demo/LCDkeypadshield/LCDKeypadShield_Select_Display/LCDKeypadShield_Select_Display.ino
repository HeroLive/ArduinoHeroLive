#include <LiquidCrystal.h>

//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;

LiquidCrystal lcd( pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// define some values used by the panel and buttons
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

void setup() {

  lcd.begin(16, 2);

  lcd.setCursor(3, 0);

  lcd.print("Hero Live");
  lcd.setCursor(0, 1);
  lcd.print("Press Key:");
}

void loop() {
  lcd_key = read_LCD_buttons();
  lcd.setCursor(10, 1);
  switch (lcd_key) {
    case btnRIGHT: {
        lcd.print(" RIGHT");
        break;
      }
    case btnLEFT: {
        lcd.print("  LEFT");
        break;
      }
    case btnUP: {
        lcd.print("    UP");
        break;
      }
    case btnDOWN: {
        lcd.print("  DOWN");
        break;
      }
    case btnSELECT: {
        lcd.print("SELECT");
        break;
      }
//    case btnNONE: {
//        lcd.print("  NONE");
//        break;
//      }
  }
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
