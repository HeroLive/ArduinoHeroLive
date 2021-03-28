#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F


//encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int enSW = 4; //The select switch for our encoder.

volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
long encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
long oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//---------
int menuCnt = 0;
int menuLength = 6;
String menu[] = {"First Menu", "Second Menu", "Third Menu", "Fourth Menu", "Fifth Menu", "Sixth Menu"};
int menuValue[] = {1, 2, 3, 4, 5, 6};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(enSW, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print(" ** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print(" >> Menu Demo <<");
  delay(3000);
  lcd.clear();

  updateMenu(menuCnt);
}

void loop() {
  //  Serial.println(menuCnt);
  if (oldEncPos != encoderPos) {
    if (oldEncPos < encoderPos) {
      menuCnt++;
    } else {
      menuCnt--;
    }
    if (menuCnt >= menuLength) {
      menuCnt = 0;
    } else if (menuCnt < 0) {
      menuCnt = menuLength-1;
    }
    oldEncPos = encoderPos;
    updateMenu(menuCnt);
    delay(100);
  }
  if (digitalRead(enSW) == 0) {
    while (digitalRead(enSW) == 0);
    settingValue(menuCnt);
  }
}

//Menu
void updateMenu(int menuSelect) {
  int select = 1;
  if (menuSelect % 2 == 0) {
    select = 2;
    if (menuSelect >= menuLength - 1) {
      select = 0;
    }
  }
  Serial.print(menuSelect);
  Serial.print("--");
  Serial.println(select);
  switch (select) {
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect]);
      lcd.setCursor(16 - numDigit(menuValue[menuSelect]), 0);
      lcd.print(menuValue[menuSelect]);
      lcd.setCursor(1, 1);
      lcd.print(menu[menuSelect + 1]);
      lcd.setCursor(16 - numDigit(menuValue[menuSelect + 1]), 1);
      lcd.print(menuValue[menuSelect + 1]);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect - 1]);
      lcd.setCursor(16 - numDigit(menuValue[menuSelect - 1]), 0);
      lcd.print(menuValue[menuSelect - 1]);
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.setCursor(1, 1);
      lcd.print(menu[menuSelect]);
      lcd.setCursor(16 - numDigit(menuValue[menuSelect]), 1);
      lcd.print(menuValue[menuSelect]);
      break;
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect]);
      lcd.setCursor(16 - numDigit(menuValue[menuSelect]), 0);
      lcd.print(menuValue[menuSelect]);
      break;
  }
}
//-------setting value of menu 0X----
void settingValue(int menuSelect) {
  updateSettingValue(menuSelect);
  while (true) {
    if (oldEncPos != encoderPos) {
      if (oldEncPos < encoderPos) {
        menuValue[menuSelect]++;
      } else {
        menuValue[menuSelect]--;
      }
      oldEncPos = encoderPos;
      updateSettingValue(menuSelect);
    }
    if (digitalRead(enSW) == 0) {
      while (digitalRead(enSW) == 0);
      updateMenu(menuCnt);
      break;
    }
  }
}
//-----update seeting value-----
void updateSettingValue(int menuSelect) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">Back");
  lcd.setCursor(1, 1);
  lcd.print(menu[menuSelect]);
  lcd.setCursor(16 - numDigit(menuValue[menuSelect]), 1);
  lcd.print(menuValue[menuSelect]);
}

//-------Encoder reading-------------
void PinA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge

    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

//---------------------------
int numDigit(int n) {
  int count = 0;
  if (n <= 0) {
    count = 1;
  }
  while (n != 0)
  {
    //sau một vòng lặp thì count sẽ tăng lên 1
    count++;
    //chia lấy nguyên cho 10, cứ một lần chia thì số n sẽ có một chữ số
    n = n / 10;
  }
  return count;
}
