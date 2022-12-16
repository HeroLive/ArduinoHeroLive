#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F
//Cam bien sieu am
#include <Ultrasonic.h>
int trigPin01 = 5;
int echoPin01 = 4;
int trigPin02 = 7;
int echoPin02 = 6;
Ultrasonic ultrasonic01(trigPin01, echoPin01);
Ultrasonic ultrasonic02(trigPin02, echoPin02);
int distance01;
int distance02;
//Servo
#include <Servo.h>
#define servoPin01 10
#define servoPin02 11
Servo servo01;
Servo servo02;

void setup() {
  Serial.begin(9600);
  // Attach the Servo variable to a pin:
  servo01.attach(servoPin01);
  servo02.attach(servoPin02);
  //lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);  //frint from column 3, row 0
  lcd.print("    Parking");
  lcd.setCursor(0, 1);
  lcd.print("  Bai gui xe   ");
  delay(5000);
  lcd.clear();
}

void loop() {
  distance01 = ultrasonic01.read();
  distance02 = ultrasonic02.read();

  Serial.print("Distance 01: ");
  Serial.print(distance01);
  Serial.print(" *** Distance 02: ");
  Serial.println(distance02);
  //gate 01
  if (distance01 < 10) {
    servo01.write(90);

    lcd.setCursor(0, 0);  //frint from column 3, row 0
    lcd.print("Xchao quy khach");
    lcd.setCursor(0, 1);
    lcd.print("  Moi xe vao  ");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);  //frint from column 3, row 0
    lcd.print("    Parking");
    lcd.setCursor(0, 1);
    lcd.print("  Bai gui xe   ");
  } else {
    servo01.write(0);
  }
  //gate 02
  if (distance02 < 10) {
    lcd.clear();
    servo02.write(90);
    lcd.setCursor(0, 0);  //frint from column 3, row 0    
    lcd.print("    Moi xe ra  ");
    lcd.setCursor(0, 1);
    lcd.print("Tbiet Quy khach");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);  //frint from column 3, row 0
    lcd.print("     Parking");
    lcd.setCursor(0, 1);
    lcd.print("   Bai gui xe   ");
  } else {
    servo02.write(0);
  }
}