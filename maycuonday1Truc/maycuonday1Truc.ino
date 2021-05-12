#include <Wire.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <MsTimer2.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
const int stepPin = A4;
const int dirPin = A5;
int encoderPin = 2;
int holdPin = 6;
unsigned long Speed = 0, RS = 0;
double delayTime;
long i;
int a = 8; //vi buoc
int b = 3; //he so banh rang
int countRound = 0;
int nRound = 0;
void setup()
{
  Serial.begin(9600);
  //  digitalWrite(3, LOW);
  pinMode(3, INPUT_PULLUP);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(encoderPin, INPUT_PULLUP);
  pinMode(holdPin, INPUT);

  attachInterrupt(1, FootSwitchMove, HIGH); // digital 3

  lcd.begin(20, 04);
  lcd.setCursor(0, 0);
  lcd.print("So vong dang quan:");
  lcd.setCursor(16, 1);
  lcd.print("(V)");
  lcd.setCursor(11, 1);
  lcd.print("/5");
  lcd.setCursor(0, 2);
  lcd.print("Vong/luot: ");
  lcd.setCursor(16, 2);
  lcd.print("(V)");
  lcd.setCursor(0, 3);
  lcd.print("Toc do   : ");
  lcd.setCursor(16, 3);
  lcd.print("(%)");
  MsTimer2::set(50, LoadADC); // ms period
  MsTimer2::start();
}
void loop()
{

}
void LoadADC()
{
  RS = analogRead(A1) / 10; // số vòng quấn dây mỗi lần quấn Max  = 1023/10 = 102
  Speed = map(analogRead(A0), 0, 1023, 0, 100);  //Tốc độ tính theo % (0% đến 100%)
  delayTime = ((60 * pow(10, 6)) * 15 / (100 * a * b * 2 * (Speed * 11 * 3)));
  lcd.setCursor(5, 1);
  lcd.print(nRound);
  lcd.setCursor(10, 1);
  lcd.print(countRound);
  lcd.setCursor(11, 2);
  lcd.print(RS);
  lcd.setCursor(11, 3);
  lcd.print(Speed);
}
void FootSwitchMove()
{
  int encoderStatus = digitalRead(encoderPin);
  Serial.println("Motor running..");
  for (i = 0; i < (RS * a * b * 100); i++)
  {
    while (digitalRead(holdPin) == HIGH) {
      Serial.println("holding..");
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);

    if (digitalRead(encoderPin) != encoderStatus && encoderStatus == 0) {
      Encoder();
    }
    encoderStatus = digitalRead(encoderPin);
  }
}

void Encoder() {
  Serial.println("Encoder");
  countRound++;
  if (countRound > 4) {
    countRound = 0;
    nRound++;
  }
  lcd.setCursor(5, 1);
  lcd.print(nRound);
  lcd.setCursor(10, 1);
  lcd.print(countRound);
}
