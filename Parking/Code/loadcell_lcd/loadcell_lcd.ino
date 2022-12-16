#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
int reading;
int lastReading;
//REPLACE WITH YOUR CALIBRATION FACTOR
#define CALIBRATION_FACTOR 408.87

//Servo
#include <Servo.h>
#define servoPin01 10
#define servoPin02 11
Servo servo01;
Servo servo02;

void displayWeight(int w) {
  int weight = w<10 ? 0 : w;
  lcd.clear();
  lcd.setCursor(0, 0);  //frint from column 3, row 0
  lcd.print("KL xe: ");
  lcd.print(weight);
  lcd.print(" g");
  if (weight < 10) {
    lcd.setCursor(0, 1);
    lcd.print("Dang cho");
    servo01.write(0);
    servo02.write(0);
  } else if (weight > 10 && weight <= 200) {
    lcd.setCursor(0, 1);
    lcd.print("Xe may  ");
    servo01.write(90);
    servo02.write(0);
  } else if (weight > 200) {
    lcd.setCursor(0, 1);
    lcd.print("Xe oto   ");
    servo01.write(0);
    servo02.write(90);
  }
}

void setup() {
  Serial.begin(9600);

  // Attach the Servo variable to a pin:
  servo01.attach(servoPin01);
  servo02.attach(servoPin02);

  //lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);  //frint from column 3, row 0
  lcd.print("    Load cell");
  lcd.setCursor(0, 1);
  lcd.print("  Bai gui xe   ");
  delay(5000);
  lcd.clear();

  displayWeight(0);

  //load cell
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);  // this value is obtained by calibrating the scale with known weights
  scale.tare();                         // reset the scale to 0
}

void loop() {

  if (scale.wait_ready_timeout(200)) {
    reading = round(scale.get_units());
    Serial.print("Weight: ");
    Serial.println(reading);
    if (reading != lastReading && abs(reading - lastReading)>4) {
      delay(1000);
      reading = round(scale.get_units(5));
      displayWeight(reading);
    }
    lastReading = reading;
  } else {
    Serial.println("HX711 not found.");
  }
}