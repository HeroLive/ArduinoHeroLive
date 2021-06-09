#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define R_IR 2
#define G_IR 3
#define R_Xilanh 11
#define G_Xilanh 12

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

#define STATE_STARTUP 0
#define STATE_GETCOLOR 1
#define STATE_CHECKIR 2
#define STATE_RELAY 3
#define STATE_COUNTER 4

byte currentState = STATE_STARTUP;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
char color = 'N';
String colors = "**";
int R_cnt = 0;
int G_cnt = 0;
int B_cnt = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(R_IR, INPUT);
  pinMode(G_IR, INPUT);
  pinMode(R_Xilanh, OUTPUT);
  pinMode(G_Xilanh, OUTPUT);
  digitalWrite(R_Xilanh, LOW);  //note relay HIGH/LOW
  digitalWrite(G_Xilanh, LOW);  //note relay HIGH/LOW
  delay(100);

  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Ghi gioi thieu va huong dan, delay 5 giay cho nguoi dung xem
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Phan loai san pham");
  delay(1000);

  lcd.clear();
  delay(1000);
  checkDataChange();
  delay(2000);
}

void loop() {
  updateState(currentState);
  //  updateLCD();
}
void updateState(byte aState) {
  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      currentState = STATE_GETCOLOR;
      break;
    case STATE_GETCOLOR:
      Serial.println("STATE_GETCOLOR");
      //      getColor();
      testbySerial();
      break;
    case STATE_CHECKIR:
      Serial.println("STATE_CHECKIR");
      checkIR();
      break;
    case STATE_RELAY:
      Serial.println("STATE_RELAY");
      xilanh();
      break;
    case STATE_COUNTER:
      Serial.println("STATE_COUNTER");
      color = 'N';
      currentState = STATE_GETCOLOR;
      checkDataChange();
      break;

  }
}

void getColor() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output frequency
  redColor = pulseIn(sensorOut, LOW);
  // Printing the RED (R) value
  //  Serial.print("R = ");
  //  Serial.print(redColor);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenColor = pulseIn(sensorOut, LOW);
  // Printing the GREEN (G) value
  //  Serial.print(" G = ");
  //  Serial.print(greenColor);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  blueColor = pulseIn(sensorOut, LOW);

  // Printing the BLUE (B) value
  //  Serial.print(" B = ");
  //  Serial.print(blueColor);
  delay(100);

  //
  Serial.print(blueColor);
  Serial.print(" ");
  Serial.print(redColor);
  Serial.print(" ");
  Serial.println(greenColor);
  // Checks the current detected color and prints
  // a message in the serial monitor
  /******/
  if (redColor > 300 && greenColor > 300 && blueColor > 300) {
    Serial.println("Bang tai");
  } else {
    if (redColor < greenColor && redColor < blueColor) {
      Serial.println(" - RED detected!");
      color = 'R';
    }
    if (greenColor < redColor && greenColor < blueColor) {
      Serial.println(" - GREEN detected!");
      color = 'G';
    }
    if (blueColor < redColor && blueColor < greenColor) {
      Serial.println(" - BLUE detected!");
      color = 'B';
    }
    currentState = STATE_CHECKIR;
  }
}

void checkIR() {
  if (digitalRead(R_IR) == 0) {
    while (digitalRead(R_IR) == 0) {
      Serial.println("Red_IR");
    }
    if (color == 'R') {
      R_cnt++;
      currentState = STATE_RELAY;
    }

  }
  if (digitalRead(G_IR) == 0) {
    while (digitalRead(G_IR) == 0) {
      Serial.println("Green_IR");
    }
    if (color == 'G') {
      G_cnt++;
      currentState = STATE_RELAY;
    } else if (color == 'B') {
      B_cnt++;
      color = 'N';
      currentState = STATE_GETCOLOR;
      checkDataChange();
    }
  }
}

void xilanh() {
  if (color == 'R') {
    digitalWrite(R_Xilanh, HIGH); //note relay HIGH/LOW
    delay(400);
    digitalWrite(R_Xilanh, LOW); //note relay HIGH/LOW
  }
  if (color == 'G') {
    digitalWrite(G_Xilanh, HIGH); //note relay HIGH/LOW
    delay(400);
    digitalWrite(G_Xilanh, LOW); //note relay HIGH/LOW
  }
  delay(1000);
  currentState = STATE_COUNTER;
}

void updateLCD() {
  //  lcd.setCursor(0, 0);
  //  lcd.print("*Phan loai san pham*");
  //  lcd.setCursor(0, 1);
  //  lcd.print("***");

  lcd.setCursor(0, 0);
  lcd.print("Red ");
  lcd.setCursor(8 - numDigit(R_cnt), 0);
  lcd.print(R_cnt);

  lcd.setCursor(14, 0);
  lcd.print(colors);

  lcd.setCursor(0, 1);
  lcd.print("Green ");
  lcd.setCursor(8 - numDigit(G_cnt), 1);
  lcd.print(G_cnt);

  lcd.setCursor(9, 1);
  lcd.print("Blue ");
  lcd.setCursor(16 - numDigit(B_cnt), 1);
  lcd.print(B_cnt);
}

void checkDataChange() {
  switch (color) {
    case 'R':
      colors = "#R";
      break;
    case 'G':
      colors = "#G";
      break;
    case 'B':
      colors = "#B";
      break;
    default:
      colors = "**";
      break;
  }
  updateLCD();

}
int numDigit(int n) {
  int count = 0;
  if (n <= 0) {
    count = 1;
  }
  while (n != 0)
  {
    //sau một vòng lặp thì count sẽ tăng lên 1
    count++;
    //chia lấy nguyên cho 10, cứ một lần chia thì số n sẽ có một chữ số
    n = n / 10;
  }
  return count;
}

void testbySerial() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    color = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(color);
    checkDataChange();
    if(color!='N'){
      currentState = STATE_CHECKIR;
    } 
  }
}
