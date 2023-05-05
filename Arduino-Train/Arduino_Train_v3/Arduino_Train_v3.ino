//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED define
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int Dout[16] = { 39, 38, 41, 40, 43, 42, 45, 44, 47, 46, 49, 48, 51, 50, 53, 52 };
long _tdelay = 500;  //200
long _t = 0;
long time[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool status[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int irPin = 8;

int count = 0;

float N = 0;
bool irStatus = 1;


void setup() {
  Serial.begin(115200);
  pinMode(irPin, INPUT_PULLUP);
  for (int i = 0; i < 16; i++) {
    pinMode(Dout[i], OUTPUT);
    digitalWrite(Dout[i], status[i]);
  }

  //For OLED I2C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  oled.display();
  delay(2000);
  // Clear the buffer
  oled.clearDisplay();

  // Draw a single pixel in white
  oled.drawPixel(10, 10, SSD1306_WHITE);
  oled.display();
}

void loop() {
  // Serial.print(irStatus);
  // Serial.println(digitalRead(irPin));
  if (digitalRead(irPin) == 0 && irStatus == 1) {
    if (N < 1000) {
      N = N + 1;
    } else N = 0;
    irStatus = digitalRead(irPin);
  } else {
    irStatus = digitalRead(irPin);
  }

  _tdelay = map(analogRead(A0), 0, 1023, 80, 500);
  // Serial.println(_tdelay);
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
  oled.clearDisplay();
  oledDisplay(6, 0, 15, N);
  oled.display();
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

void oledDisplay(int size, int x, int y, float value) {
  oled.setTextSize(size);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(x, y);
  oled.print(value, 0);
}
