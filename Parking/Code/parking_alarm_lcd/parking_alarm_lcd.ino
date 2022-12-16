#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F

// Gọi thư viện DHT11
#include "DHT.h"
const int DHTPIN = 13;      //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);
//buzzer
int buzzer = 12;

void setup() {
  Serial.begin(9600);
  //dht
  dht.begin();
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

void displayDHT(int t, int h) {
  Serial.print("Nhiet do: ");
  Serial.print(t);  //Xuất nhiệt độ
  Serial.print(" *** Do am: ");
  Serial.println(h);  //Xuất độ ẩm
}

void displaySensor() {


  
}

void loop() {
  float h = dht.readHumidity();     //Đọc độ ẩm
  float t = dht.readTemperature();  //Đọc nhiệt độ
  displayDHT(t,h);
  delay(100);
}