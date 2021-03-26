#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

//encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int enSW = 9; //The select switch for our encoder.

volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
int encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
int oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//---------

int ledPin = 13; // set the signal pin out here.
int flashButton = 8; //
int pulseNum = 100; // set the Pulse number here.
int pulseDuration = 50; // set the Pulse width here.

#define STATE_STARTUP 0
#define STATE_MAINMENU 1
#define STATE_SETTING 2
#define STATE_WAITTORUN 3
#define STATE_FLASH 4

byte currentState = STATE_STARTUP;

//------
int mainMenuCnt = 1;
int settingMenuCnt = 1;
int numOfSetting = 2;


void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(flashButton, INPUT_PULLUP);
  digitalWrite(ledPin, HIGH);

  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(enSW, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0); //frint from column 3, row 0
  lcd.print("Hero Live");
  lcd.setCursor(0, 1);
  lcd.print("Xin chao cac ban");
  delay(2000);
  lcd.clear();

  updateState(STATE_MAINMENU);
}

void loop()
{
  //  if (oldEncPos != encoderPos) {
  //    Serial.println(encoderPos);
  //    oldEncPos = encoderPos;
  //  }
}




// ---------------------------
void updateState(byte aState)
{
  if (aState == currentState)
  {
    return;
  }

  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      delay(1000);
      break;
    case STATE_MAINMENU:
      Serial.println("STATE_MAINMENU");
      mainMenu();
      break;
    case STATE_SETTING:
      delay(1000);
      setting();
      break;
    case STATE_WAITTORUN:
      waitToRun();
      break;
    case STATE_FLASH:
      delay(100);
      Serial.println("STATE_FLASH");
      flash(pulseNum, pulseDuration);
      break;
  }

  currentState = aState;
}
//--------------------------------
void mainMenu() {
  //display line 0
  int currentEncoderPos = encoderPos;
  Serial.println(currentEncoderPos);
  updateMainMenu();
  while (true) {
    Serial.print(currentEncoderPos);
    Serial.print(" ");
    Serial.print(encoderPos);
    Serial.print(" ");
    Serial.println(mainMenuCnt);

    if (currentEncoderPos != encoderPos) {
      mainMenuCnt = mainMenuCnt + (encoderPos - currentEncoderPos);
      currentEncoderPos = encoderPos;
      updateMainMenu();
    }
    if (digitalRead(enSW) == 0) {
      while (digitalRead(enSW) == 0);
      if (mainMenuCnt == 1) {
        updateState(STATE_SETTING);
      } else if (mainMenuCnt == 2) {
        updateState(STATE_WAITTORUN);
      }
    }
  }

}
// ---------------------------
void setting() {
  long timewait = millis();
  Serial.println("STATE_SETTING");
  int currentEncoderPos = encoderPos;
  updateSettingMenu();
  while (true) {
    Serial.println(encoderPos);
    if (digitalRead(enSW) == 0) {
      timewait = millis();
      while (digitalRead(enSW) == 0);
      settingMenuCnt++;
      if (settingMenuCnt > numOfSetting) {
        settingMenuCnt = 1;
      }
      updateSettingMenu();
    }
    if (currentEncoderPos != encoderPos) {
      timewait = millis();
      switch (settingMenuCnt) {
        case 1:
          pulseNum = pulseNum + 10 * (encoderPos - currentEncoderPos);
          if (pulseNum < 0) {
            pulseNum = 0;
          }
          break;
        case 2:
          pulseDuration = pulseDuration + (encoderPos - currentEncoderPos);
          if (pulseDuration < 0) {
            pulseDuration = 0;
          }
          break;
      }
      currentEncoderPos = encoderPos;
      updateSettingMenu();
    }
    if (millis() - timewait > 5000) {
      updateState(STATE_MAINMENU);
    }
  }
}

void waitToRun() {
  long timewait = millis();
  Serial.println("STATE_WAITTORUN");
  lcd.clear();
  lcd.setCursor(5, 0); //frint from column 1, row 0
  lcd.print("FLASH");
  lcd.setCursor(1, 1);
  lcd.print("Wait to run");
  while (true) {
    //    Serial.println(digitalRead(flashButton));
    if (digitalRead(flashButton) == 0) {
      while (digitalRead(flashButton) == 0);
      updateState(STATE_FLASH);
    }
    if (millis() - timewait > 5000) {
      updateState(STATE_MAINMENU);
    }
  }
}

void flash(int repeats, int pulseDuration)
{
  lcd.clear();
  lcd.setCursor(5, 0); //frint from column 1, row 0
  lcd.print("FLASH");
  lcd.setCursor(0, 1);
  lcd.print("Running ......");
  for (int i = 0; i < repeats; i++)
  {
    digitalWrite(ledPin, LOW);
    delay(pulseDuration);
    digitalWrite(ledPin, HIGH);
    delay(pulseDuration);
  }
  updateState(STATE_WAITTORUN);
}

//--------------------
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
//-------------------
void updateMainMenu() {
  switch (mainMenuCnt) {
    case 0:
      mainMenuCnt = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0); //frint from column 1, row 0
      lcd.print(">Setting");
      lcd.setCursor(1, 1);
      lcd.print("Flash");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(1, 0); //frint from column 0, row 0
      lcd.print("Setting");
      lcd.setCursor(0, 1);
      lcd.print(">Flash");
      break;
    case 3:
      mainMenuCnt = 2;
      break;
  }
}
//-------------------
void updateSettingMenu() {
  switch (settingMenuCnt) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0); //frint from column 1, row 0
      lcd.print(">PulseNum");
      lcd.setCursor(11, 0);
      lcd.print(pulseNum);
      lcd.setCursor(1, 1);
      lcd.print("PulseDur");
      lcd.setCursor(11, 1);
      lcd.print(pulseDuration);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(1, 0); //frint from column 1, row 0
      lcd.print("PulseNum");
      lcd.setCursor(11, 0);
      lcd.print(pulseNum);
      lcd.setCursor(0, 1);
      lcd.print(">PulseDur");
      lcd.setCursor(11, 1);
      lcd.print(pulseDuration);
      break;
  }
}
