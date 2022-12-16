#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // or 0x3F
//Cam bien sieu am
#include <Ultrasonic.h>
int trigPin01 = 3;
int echoPin01 = 2;
Ultrasonic ultrasonic01(trigPin01, echoPin01);
int distance01;
//Servo
void setup()
{
  Serial.begin(9600);
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(3,0); //frint from column 3, row 0
  lcd.print("Hero Live");
  lcd.setCursor(0,1);
  lcd.print("Xin chao cac ban");
  delay(5000);

}

void loop()
{
  distance01 = ultrasonic01.read();

  Serial.print("Distance 01 in CM: ");
  Serial.println(distance01);
  delay(1000);
  
}