#include <TM1637Display.h>
// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

int tempC = 32;
const uint8_t doC[] = {
  SEG_A | SEG_B | SEG_G | SEG_F,           // o
  SEG_A | SEG_D | SEG_E | SEG_F    // C          
  };
int hour = 12;
int minu = 15;
int timeDisplay = hour*100 + minu; //1215
void setup() {
 display.setBrightness(4);
 display.clear();
 delay(1000);
}

void loop() {
// display.showNumberDec(tempC,false,2,0);
// display.setSegments(doC,2,2);
  display.showNumberDecEx(timeDisplay,0b01000000);
}
