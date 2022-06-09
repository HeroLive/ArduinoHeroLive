#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int address = 0;
byte value ;
const byte button1 = 2, button2 = 3;
const byte  relay1 = 4, relay2 = 5, relay3 = 6, relay4 = 7;
//volatile int STATUS = LOW;
int i, j, n, m;
bool ON = HIGH;
bool OFF = LOW;
int count2 = 0;
int count1 = 0;

int run_status = 0;

bool delay_status1 = false;
bool delay_status2 = false;

long time1, time2, time3 = 0;

int Cycle_Eu, Cycle_Re, count_Re, count_Eu;

int Limit1 = 5;
int Limit2 = 10;
int timecho = 2000;
unsigned long hientai = 0;
unsigned long thoigian;
void setup()
{
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(relay1, OUTPUT); // REINDEER
  pinMode(relay2, OUTPUT); // REINDEER
  pinMode(relay3, OUTPUT); // EUROSTAR
  pinMode(relay4, OUTPUT); // EUROSTAR
  digitalWrite(relay2, ON);
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PCBA REINDEER");
  lcd.setCursor(0, 1);
  lcd.print("COUNT:");
  lcd.setCursor(0, 2);
  lcd.print("PCBA EUROSTAR ");
  lcd.setCursor(0, 3);
  lcd.print("COUNT:");
  lcd.setCursor(10, 1);
  lcd.print("Cycle:");
  lcd.setCursor(10, 3);
  lcd.print("Cycle:");
  run_status = 0;
  time1 = millis();
  time2 = millis();
  Serial.println("Push button start to Start");
}

void loop()
{
  //wait start

  if (digitalRead(button1) == LOW) { //Start
    while(digitalRead(button1) == LOW);
    run_status = 1;
    Serial.println("Start");
  }
  //Serial.println("running");
  if (digitalRead(button2) == LOW) //STOP
  {
    while (1)
    {
      Serial.println("Pause");
      if (digitalRead(button1) == LOW)
        break;
    }
  }


  if ((millis() - time1 >= 1000) && (run_status == 1) )
  {
    time1 = millis();
    delay_status1 = !delay_status1;
    if (delay_status1) {
      Serial.println("delay1 ON");
      Serial.println("delay2 ON");
    } else {
      Serial.println("delay2 OFF");
      count1++;
      Serial.print("count1: ");
      Serial.println(count1);
    }
  }


  if (count1 >= Limit1)
  {
    Serial.println("delay1 OFF");
    run_status = 2;
    count2++;
    Serial.print("count2: ");
    Serial.println(count2);
    count1 = 0;
    time2 = millis();
    Serial.println("Chuyen ve run_status == 2");
  }

  if ((millis() - time2 >= 1000) && (run_status == 2) )
  {
    run_status = 1;
    Serial.println("Chuyen ve run_status == 1");
    time1 = millis();
  }
  if (count2 >= Limit2)
  {
    count2 = 0;
    while (1)
    {
      Serial.println("STOP");
      if (digitalRead(button1) == LOW)
        break;

    }
  }
}
