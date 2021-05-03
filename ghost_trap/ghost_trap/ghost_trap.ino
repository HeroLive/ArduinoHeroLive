#include <Servo.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define ringNeo 8
#define ringNUMPIXELS 8
Adafruit_NeoPixel ringPixels(ringNUMPIXELS, ringNeo, NEO_GRB + NEO_KHZ800);

#define barNeo 9
#define barNUMPIXELS 8
Adafruit_NeoPixel barPixels(barNUMPIXELS, barNeo, NEO_GRB + NEO_KHZ800);

Servo servo1; 
Servo servo2;

#define STATE_INIT 0
#define STATE_STARTUP 1
#define STATE_WAITTING 2
#define STATE_OPENTRAP 3
#define STATE_CLOSETRAP 4
#define STATE_PROCESSTRAP 5
#define STATE_REPLAYTRAP 6

int led = 13;
int bt1 = 2;
byte currentState = STATE_INIT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ringPixels.begin();
  barPixels.begin();
  servo1.attach(5);
  servo2.attach(6);
  pinMode(led, OUTPUT);
  pinMode(bt1, INPUT_PULLUP);

  ring_pixels_off();
  bar_pixels_off();
  digitalWrite(led, HIGH);
  servo1.write(0);
  servo2.write(0);
  updateState(STATE_STARTUP);
}

void loop() {
  // put your main code here, to run repeatedly:

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
      digitalWrite(led, HIGH);
      playAudio(1);
      stopAudio(1);
      updateState(STATE_WAITTING);
      break;
    case STATE_WAITTING:
      Serial.println("STATE_WAITTING");
      while (true) {
        if (digitalRead(bt1) == 0) {
          while (digitalRead(bt1) == 0);
          updateState(STATE_OPENTRAP);
          break;
        }
      }
    case STATE_OPENTRAP:
      Serial.println("STATE_OPENTRAP");
      open_trap();
      break;
    case STATE_CLOSETRAP:
      Serial.println("STATE_CLOSETRAP");
      close_trap();
      break;
    case STATE_PROCESSTRAP:
      Serial.println("STATE_PROCESSTRAP");
      process_trap();
      break;
    case STATE_REPLAYTRAP:
      Serial.println("STATE_REPLAYTRAP");
      replay_process_trap();
      break;
  }

  currentState = aState;
}
//--------------------------------
void open_trap() {
  servo_on();

  playAudio(2);
  while (true) {
    RGB_blink("R");
    if (digitalRead(bt1) == 0) {
      while (digitalRead(bt1) == 0);
      stopAudio(2);
      updateState(STATE_CLOSETRAP);
      break;
    }
  }
}

void close_trap() {
  long t = millis();
  playAudio(3);
  while (millis() - t < 3000) {
    RGB_blink("W");
  }
  stopAudio(3);
  servo_off();
  bar_graph_fill();
  updateState(STATE_PROCESSTRAP);
}

void process_trap() {
  long t = millis();
  ring_pixels_off();
  playAudio(4);
  while (millis() - t < 2000) {
    led_blink();
  }
  updateState(STATE_REPLAYTRAP);
}

void replay_process_trap() {
  ring_pixels_on("W");
  playAudio(5);
  updateState(STATE_PROCESSTRAP);
}

void servo_on() {
  Serial.println("servo_on");
  servo1.write(90);
  servo2.write(90);
}
void servo_off() {
  Serial.println("servo_off");
  servo1.write(0);
  servo2.write(0);
}

void led_blink() {
  if ((millis() * 3 / 1000) % 2 == 0) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

void RGB_blink(char *c) {
  Serial.print("RGB_blink ");
  Serial.println(c);
  if ((millis() * 3 / 1000) % 2 == 0) {
    ring_pixels_on(c);
  } else {
    ring_pixels_off();
  }
}
void ring_pixels_off() {
  ringPixels.clear();
  for (int i = 0; i < ringNUMPIXELS; i++)
  {
    ringPixels.setPixelColor(i, 0, 0, 0);
  }
  ringPixels.show();
}
void ring_pixels_on(char *c) {
  int R = 0;
  int G = 0;
  int B = 0;
  if (c == "R") {
    R = 255;
  } else if (c == "G") {
    G = 255;
  } else if (c == "B") {
    B = 255;
  } else if (c == "W") {
    R = 255;
    G = 255;
    B = 255;
  }
  ringPixels.clear();
  ringPixels.setBrightness(10);
  ringPixels.setPixelColor(0, ringPixels.Color(R, G, B));
  ringPixels.setPixelColor(3, ringPixels.Color(R, G, B));
  ringPixels.setPixelColor(6, ringPixels.Color(R, G, B));
  ringPixels.show();
}

void bar_pixels_off() {
  barPixels.clear();
  for (int i = 0; i < barNUMPIXELS; i++)
  {
    barPixels.setPixelColor(i, 0, 0, 0);
  }
  barPixels.show();
}
void bar_pixels_on(int num) {
  barPixels.setPixelColor(num, 185, 150, 0);
  barPixels.show();
}

void bar_graph_fill() {
  Serial.println("bar_graph_fill");
  for (int i = 0; i < barNUMPIXELS; i++) {
    bar_pixels_on(i);
    delay(100);
  }
  bar_pixels_off();
  bar_pixels_on(7);
}

void playAudio(int track) {
  Serial.print("Play track ");
  Serial.println(track);
}
void stopAudio(int track) {
  Serial.print("Stop track ");
  Serial.println(track);
}
