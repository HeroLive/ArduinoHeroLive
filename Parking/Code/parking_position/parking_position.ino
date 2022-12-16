#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F

byte SENSORS = 0;
const int numSensor = 8; //so cam bien tiem can
bool readSensors[numSensor] = { 1, 1, 1, 1, 1, 1, 1, 1 }; //gia tri doc tu cam bien: 0:co vat can, 1: ko vat can
int sensorPin[numSensor] = { 4, 5, 6, 7, 8, 9, 10, 11 }; //khai bao chan cam bien tu cam bien 1 den 8

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numSensor; i++) {
    pinMode(sensorPin[i], INPUT_PULLUP);
  }

  //lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);  //frint from column 3, row 0
  lcd.print("    Parking");
  lcd.setCursor(0, 1);
  lcd.print("  Vi tri xe");
  delay(5000);
}

void printBinary(byte inByte) {
  for (int i = 0; i < numSensor; i++) {
    Serial.print(bitRead(inByte, i));
  }
}

void displaySensor(byte s) {
  printBinary(s);
  Serial.println();

  lcd.setCursor(0, 0);
  lcd.print("P1 2 3 4 5 6 7 8");
  lcd.setCursor(0, 1);
  lcd.print("");
  for (int i = 0; i < numSensor; i++) {
    if(bitRead(s, i)){
      lcd.print(" .");
    }else {
      lcd.print(" X");
    }
  } 

  delay(500);
}

void loop() {
  for (int i = 0; i < numSensor; i++) {
    readSensors[i] = digitalRead(sensorPin[i]);
  }
  for (int i = 0; i < numSensor; i++) {
    if (readSensors[i]) {
      bitSet(SENSORS, i);
    } else {
      bitClear(SENSORS, i);
    }
  }
  displaySensor(SENSORS);
}