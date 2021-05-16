#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F
#include <AccelStepper.h>

// Define a stepper and the pins it will use
int stepPinX = 9;
int dirPinX = 10;
int stepPinY = 11;
int dirPinY = 12;
int stepPinZ = 4;
int dirPinZ = 7;
int en = 8;

AccelStepper stepperX(AccelStepper::DRIVER, stepPinX, dirPinX);
AccelStepper stepperY(AccelStepper::DRIVER, stepPinY, dirPinY);
AccelStepper stepperZ(AccelStepper::DRIVER, stepPinZ, dirPinZ);


//encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int enSW = 5; //The select switch for our encoder.

volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
long encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
long oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//---------
int menuCnt = 0;
int menuLength = 3;
String menu[] = {"X", "Y", "Z"};
int menuValue[] = {1, 2, 3};
long positions[] = {0, 0, 0};

long microStep = 1;
long fullStep = 200;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(enSW, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  //
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Stepper Control");
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
      menuCnt = menuLength - 1;
    }
    oldEncPos = encoderPos;
    updateMenu(menuCnt);
    delay(100);
  }
  if (digitalRead(enSW) == 0) {
    long t = millis();
    while (digitalRead(enSW) == 0);
    if ((millis() - t) < 1000) {
      settingValue(menuCnt);
    } else {
      Serial.println("long press");
      switch (menuCnt) {
        case 0:
          runStepper(stepperX);
          break;
        case 1:
          runStepper(stepperY);
          break;
        case 2:
          runStepper(stepperZ);
          break;
      }

    }

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
  //  Serial.print(menuSelect);
  //  Serial.print("--");
  //  Serial.println(select);
  switch (select) {
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect]);
      lcd.setCursor(9 - numDigit(menuValue[menuSelect]), 0);
      lcd.print(menuValue[menuSelect]);
      lcd.setCursor(16 - numDigit(positions[menuSelect]), 0);
      lcd.print(positions[menuSelect]);
      lcd.setCursor(1, 1);
      lcd.print(menu[menuSelect + 1]);
      lcd.setCursor(9 - numDigit(menuValue[menuSelect + 1]), 1);
      lcd.print(menuValue[menuSelect + 1]);
      lcd.setCursor(16 - numDigit(positions[menuSelect + 1]), 1);
      lcd.print(positions[menuSelect + 1]);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect - 1]);
      lcd.setCursor(9 - numDigit(menuValue[menuSelect - 1]), 0);
      lcd.print(menuValue[menuSelect - 1]);
      lcd.setCursor(16 - numDigit(positions[menuSelect - 1]), 0);
      lcd.print(positions[menuSelect - 1]);
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.setCursor(1, 1);
      lcd.print(menu[menuSelect]);
      lcd.setCursor(9 - numDigit(menuValue[menuSelect]), 1);
      lcd.print(menuValue[menuSelect]);
      lcd.setCursor(16 - numDigit(positions[menuSelect]), 1);
      lcd.print(positions[menuSelect]);
      break;
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print(menu[menuSelect]);
      lcd.setCursor(9 - numDigit(menuValue[menuSelect]), 0);
      lcd.print(menuValue[menuSelect]);
      lcd.setCursor(16 - numDigit(positions[menuSelect]), 0);
      lcd.print(positions[menuSelect]);
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
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detentom  rotation
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
//------------------------
void runStepper(AccelStepper stepper)
{
  long newPositions = menuValue[menuCnt] * microStep * fullStep;
  long currentPositions = positions[menuCnt]* microStep * fullStep;
  stepper.setCurrentPosition(currentPositions);
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
  stepper.runToNewPosition(newPositions);
  positions[menuCnt] = stepper.currentPosition() / (microStep * fullStep);
  updateMenu(menuCnt);
}
