/*CONNECTIONS:
  DS1302 CLK/SCLK --> 10
  DS1302 DAT/IO --> 9
  DS1302 RST/CE --> 8
  DS1302 VCC --> 3.3v - 5v
  DS1302 GND --> GND
  Step pulse --> 5
  Step direction --> 6
  Step enable --> 7
  Day of week:
   1: Monday
   2: Tuesday
   3: Wednesday
   4: Thursday
   5: Friday
   6: Saturday
   7: Sunday
*/
#include <ThreeWire.h>
#include <RtcDS1302.h>
#define LED 13

ThreeWire myWire(9, 10, 8); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

int _dayOfWeek = 6, _hour = 17, _min = 28, _sec = 0; //set time to run Motor
long t = 0;
boolean _dir = LOW; //change direction when start
int PUL = 5;
int DIR = 6;
int EN = 7;

int microStep = 16;
float angle = 1.8;
int targetAngle = 180;
long nSteps = (360 / angle) * microStep * targetAngle / 360;
long speeds = 300; //us - motor speed

void setup ()
{
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);

  digitalWrite(EN, LOW);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();
  if (millis() - t >= 1000) {
    printDateTime(now);
    t = millis();
  }
  if (now.DayOfWeek() == _dayOfWeek && now.Hour() == _hour && now.Minute() == _min && now.Second() == _sec) {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    runMotor();
  }

  if (!now.IsValid())
  {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }
}

void runMotor() {
  digitalWrite(LED, HIGH);
  Serial.print("Move to ");
  Serial.println(targetAngle);
  digitalWrite(DIR, _dir);
  for (int i = 1; i < nSteps; i++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(speeds);
    digitalWrite(PUL, LOW);
    delayMicroseconds(speeds);
  }
  delay(500);
  Serial.println("Move back 0");
  digitalWrite(DIR, !_dir);
  for (int i = 1; i < nSteps; i++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(speeds);
    digitalWrite(PUL, LOW);
    delayMicroseconds(speeds);
  }
  digitalWrite(LED, LOW);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
  char datestring[50];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u %02u/%02u/%04u %02u:%02u:%02u"),
             dt.DayOfWeek(),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.println(datestring);
}
