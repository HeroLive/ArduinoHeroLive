#include <Wire.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <MsTimer2.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
const int stepPin = A4;
const int dirPin = A5;
unsigned long Speed = 0, RS = 0;
double delayTime;
long i;
int a = 8; //vi buoc
int b = 3; //he so banh rang
void FootSwitchMove()
{
  for (i = 0; i < (RS * a * b * 100); i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}
void setup()
{
  Serial.begin(9600);
  digitalWrite(3, LOW);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, FootSwitchMove, HIGH);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  lcd.begin(20, 04);
  lcd.setCursor(0, 1);
  lcd.print("Vong/luot: ");
  lcd.setCursor(16, 1);
  lcd.print("(v)0");
  lcd.setCursor(0, 2);
  lcd.print("Toc do: ");
  lcd.setCursor(15, 2);
  lcd.print("(v/p)");
  MsTimer2::set(50, LoadADC); // ms period
  MsTimer2::start();
}
void loop()
{
}
void LoadADC()
{
  if (Mode==0)
{
  RS = analogRead(A1)/10;   // số vòng quấn dây mỗi lần quấn Max  = 1023/10 = 102
  Speed = map(analogRead(A0)/10) ;  //Tốc độ tính theo % (10% đến 100%)
  delayTime =((60 *pow(10,6))*15/(100*a*b*2*(Speed*11*3)));
  lcd.setCursor(11, 1);
  lcd.print("    ");
  lcd.setCursor(11, 1);
  lcd.print(RS);
  lcd.setCursor(11, 2);
  lcd.print("    ");
  lcd.setCursor(11, 2);
  lcd.print(Speed);
}
