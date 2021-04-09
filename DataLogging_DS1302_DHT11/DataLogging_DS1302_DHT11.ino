// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 3
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

//DHT11 VCC --> 5V
//DHT11 GND --> GND
//DHT11 OUT --> 2
//SD card attached to SPI bus as follows:
//SD MOSI - pin 11
//SD MISO - pin 12
//SD CLK - pin 13
//SD CS - pin 10

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "DHT.h"
#include <SPI.h>
#include <SD.h>

ThreeWire myWire(4, 5, 3); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

const int DHTPIN = 2;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;

File myFile;
const int chipSelect = 10;
String fileName = "datalog.txt";

void setup ()
{
  Serial.begin(9600);

  dht.begin();

  rtcSetting();

  sdSetting();

  delay(1000);

}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();
  String dateTime = printDateTime(now);
  if (!now.IsValid())
  {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }

  h = dht.readHumidity(); //Đọc độ ẩm
  t = dht.readTemperature(); //Đọc nhiệt độ
  String ht = printDHT(h, t);
  writeToSD(fileName,dateTime+" "+ht);
  delay(5000); // five seconds
}


void rtcSetting() {
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

void sdSetting() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file for reading:
  myFile = SD.open(fileName);
  if (myFile) {
    Serial.println(fileName);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
  }

  // delete the file:
//  Serial.println("Removing...");
//  SD.remove(fileName);

//  writeToSD(fileName, "Start write data to file");

}

void writeToSD(String fileName, String str) {
  File dataFile = SD.open(fileName, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(str);
    dataFile.close();
    // print to the serial port too:
    Serial.print("Saved: ");
    Serial.println(str);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
}

#define countof(a) (sizeof(a) / sizeof(a[0]))
String printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
  return datestring;
}

String printDHT(float h, float t) {
  Serial.print("\tNhiet do: ");
  Serial.print(t); //Xuất nhiệt độ
  Serial.print("\tDo am: ");
  Serial.println(h);
  return String(h) + " " + String(t);
}
