#define CUSTOM_SETTINGS
#define INCLUDE_LEDCONTROL_MODULE
//#include <evive.h>
#include <Dabble.h>

void setup() {
Serial.begin(115200);
Dabble.begin(250000);     //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
  
}

void loop() {
Dabble.processInput();
Serial.print("Led:");
Serial.print(LedControl.getpinNumber());
Serial.print('\t');
Serial.print("State:");          //0 if led is Off. 1 if led is On.
Serial.print(LedControl.getpinState());
Serial.print('\t');
Serial.print("Brightness:");
Serial.println(LedControl.readBrightness());
}
