#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // or 0x3F
float x = 10.53;
float y = 19.25;
int counter = 0;
void setup()
{
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(3,0); //frint from column 3, row 0
  lcd.print("Hero Live");
  lcd.setCursor(0,1);
  lcd.print("Xin chao cac ban");
  delay(200000);
  lcd.clear();
}

void loop()
{
  counter = counter+1;
  //display line 0
  lcd.setCursor(0,0);
  lcd.print("So dem: ");
  lcd.setCursor(8,0);
  lcd.print(counter);

  //display line 1
  lcd.setCursor(0,1);
  lcd.print("X: ");
  lcd.setCursor(2,1);
  lcd.print(x);
  lcd.setCursor(8,1);
  lcd.print("Y: ");
  lcd.setCursor(10,1);
  lcd.print(y,1);
  delay(1000);
}
