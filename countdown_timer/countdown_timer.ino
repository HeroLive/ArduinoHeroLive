//https://www.makerguides.com/tm1637-arduino-tutorial/
#include <TM1637Display.h>
#include <EEPROM.h>

#define STATE_STARTUP 0
#define STATE_MIN 1
#define STATE_SEC 2
#define STATE_STOP 3
#define STATE_WAITRUN 4
#define STATE_RUN 5
#define STATE_ALARM 6
#define STATE_KEEPMODE 7

byte currentState = STATE_STARTUP;

bool debug = false;

int buzzer = 9;
int upPin = 6;
int modePin = 7;
int downPin = 8;

bool isStored = true; //true: luu vao eeprom
unsigned int min_addr = 0;  //0 -> 65,535
unsigned int sec_addr = 4;

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2
TM1637Display display(CLK, DIO);

long minSet = 0;
long secSet = 10;
bool isTimeOut = false;
long timeRun = 0;
long timeTriger = millis();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  Serial.println("Countdown timer app");
  display.setBrightness(4);
  display.clear();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  updateState(currentState);
//  Serial.print("minSet: ");
//  Serial.print(minSet);
//  Serial.print(" secSet: ");
//  Serial.print(secSet);
//  Serial.print(" timeRun: ");
//  Serial.println(timeRun);
}

void updateState(byte aState) {
  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      currentState = STATE_WAITRUN;
      if (isStored) {
        minSet = EEPROMReadlong(min_addr);
        secSet = EEPROMReadlong(sec_addr);
      }
      if (debug) {
        Serial.println("STATE_STARTUP");
      }
      break;
    case STATE_MIN:
      updateLED();
      setTime(currentState);
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        delay(300);
        currentState = STATE_SEC;
      }
      if (debug) {
        Serial.println("STATE_MIN");
      }
      break;
    case STATE_SEC:
      updateLED();
      setTime(currentState);
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        delay(300);
        currentState = STATE_WAITRUN;
        if (isStored) {
          EEPROMWritelong(min_addr, minSet);
          EEPROMWritelong(sec_addr, secSet);
          if (debug) {
            Serial.println("Stored to EEPROM");
          }
        }
      }
      if (debug) {
        Serial.println("STATE_SEC");
      }
      break;
    case STATE_WAITRUN:
      updateLED();
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        delay(300);
        currentState = STATE_MIN;
      } else if (digitalRead(upPin) == 0) {
        while (digitalRead(upPin) == 0);
        currentState = STATE_RUN;
        timeTriger = millis();
      }
      if (debug) {
        Serial.println("WAITRUN");
      }
      break;
    case STATE_RUN:
      timeRun = millis() - timeTriger;
      updateLED();
      if (timeOut()) {
        if (debug) {
          Serial.println("Time over");
        }
        currentState = STATE_ALARM;
      }
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        delay(500);
        currentState = STATE_KEEPMODE;
      }
      if (debug) {
        Serial.println("STATE_RUN");
      }
      break;
    case STATE_KEEPMODE:
      timeRun = millis() - timeTriger;
      updateLED();
      if (timeOut()) {
        if (debug) {
          Serial.println("Time over");
        }
        currentState = STATE_ALARM;
      }
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        delay(500);
        currentState = STATE_WAITRUN;
      } else if (digitalRead(upPin) == 0) {
        while (digitalRead(upPin) == 0);
        currentState = STATE_RUN;
      }
      if (debug) {
        Serial.println("STATE_KEEPMODE");
      }
      break;
    case STATE_ALARM:
      updateLED();
      if (debug) {
        Serial.println("STATE_ALARM");
      }
      digitalWrite(buzzer, HIGH);
      if (digitalRead(modePin) == 0) {
        while (digitalRead(modePin) == 0);
        currentState = STATE_WAITRUN;
        digitalWrite(buzzer, LOW);
      }
      break;
  }
}
void setTime(byte currentState) {
  int count = 0;
  if (digitalRead(upPin) == 0) {
    while (digitalRead(upPin) == 0);
    count++;
  } else if (digitalRead(downPin) == 0) {
    while (digitalRead(downPin) == 0);
    count--;
  }
  switch (currentState) {
    case STATE_MIN:
      minSet = minSet + count;
      minSet = (minSet > 0 && minSet <= 60) ? minSet : 0;
      break;
    case STATE_SEC:
      secSet = secSet + count;
      secSet = (secSet > 0 && secSet < 60) ? secSet : 0;
      break;
  }
}
long timeToDisPlayDec(long t) {
  long min_t = 0;
  long sec_t = 0;
  min_t = (minSet * 60 + secSet - t / 1000) / 60;
  sec_t = (minSet * 60 + secSet - t / 1000) % 60;
  return min_t * 100 + sec_t;
}
void updateLED() {
  uint8_t segOff[] = {false, false};
  long timeSetToDec = minSet * 100 + secSet;
  long timeRunToDec = timeToDisPlayDec(timeRun);
  if (debug) {
    Serial.println(timeRunToDec);
  }
  switch (currentState) {
    case STATE_MIN:
      if ((millis() * 5 / 1000) % 2 == 0) {
        display.setSegments(segOff, 2, 0);
      } else {
        display.showNumberDecEx(timeSetToDec, 0b01000000, true, 4, 0);
      }
      break;
    case STATE_SEC:
      if ((millis() * 5 / 1000) % 2 == 0) {
        display.setSegments(segOff, 2, 2);
      } else {
        display.showNumberDecEx(timeSetToDec, 0b01000000, true, 4, 0);
      }
      break;
    case STATE_WAITRUN:
      if ((millis() * 5 / 1000) % 2 == 0) {
        display.showNumberDecEx(timeSetToDec, 0b10000000, true, 4, 0);
      } else {
        display.showNumberDecEx(timeSetToDec, 0b01000000, true, 4, 0);
      }
      break;
    case STATE_RUN:
      display.showNumberDecEx(timeRunToDec, 0b01000000, true, 4, 0);
      break;
    case STATE_ALARM:
      if ((millis() * 5 / 1000) % 2 == 0) {
        display.clear();
      } else {
        display.showNumberDecEx(timeRunToDec, 0b01000000, true, 4, 0);
      }
      break;
    case STATE_KEEPMODE:
      if ((millis() * 5 / 1000) % 2 == 0) {
        display.showNumberDecEx(timeRunToDec, 0b10000000, true, 4, 0);
      } else {
        display.showNumberDecEx(timeRunToDec, 0b01000000, true, 4, 0);
      }
      break;
  }

  //  //  display.showNumberDec(secSet, false, 2, 2);
  //  delay(1000);
  //  if(timeOut()){
  //    Serial.println("Time over");
  //  }
}
bool timeOut() {
  bool isTimeOut = false;
  if (timeRun > ((minSet * 60 + secSet) * 1000)) {
    isTimeOut = true;
  }
  return isTimeOut;
}

/*EEPROM WRITE/READ*/
//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
