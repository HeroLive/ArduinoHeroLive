//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int Dout[16] = { 39, 38, 41, 40, 43, 42, 45, 44, 47, 46, 49, 48, 51, 50, 53, 52 };
long _tdelay = 500; //200
long _t = 0;
long time[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool status[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int count = 0;
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 16; i++) {
    pinMode(Dout[i], OUTPUT);
    digitalWrite(Dout[i], status[i]);
  }
}

void loop() {
  // for (int i = 0; i < 16; i++) {
  //   if (digitalRead(Din[i])==0) {
  //     status[i] = 0;
  //     time[i] = millis();
  //   }
  //   if (millis() - time[i] > _tdelay) {
  //     status[i] = 1;
  //   }
  //   digitalWrite(Dout[i], status[i]);
  //   Serial.print(status[i]);
  //   if(i==15){
  //     Serial.println();
  //   } else {
  //     Serial.print(" ");
  //   }
  // }
  
  _tdelay = map(analogRead(A0),0,1023,100,500);
  Serial.println(_tdelay);
  for (int i = 0; i < 16; i++) {
    digitalWrite(Dout[i], status[i]);
  }
  status[count] = 0;
  // printArray();
  if (millis() - _t > _tdelay) {
    _t = millis();
    count = count == 15 ? 0 : count + 1;
    setBit();
  }
}

void setBit() {
  for (int i = 0; i < 16; i++) {
    status[i] = 1;
  }
}
void printArray() {
  for (int i = 0; i < 16; i++) {
    Serial.print(status[i]);
    if (i == 15) {
      Serial.println();

    } else {
      Serial.print(" ");
    }
  }
}
