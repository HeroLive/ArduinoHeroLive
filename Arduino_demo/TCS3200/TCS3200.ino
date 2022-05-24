/*
  Color Sensor      Arduino
  -----------      --------
  VCC               5V
  GND               GND
  s0                8
  s1                9
  s2                12
  s3                11
  OUT               10
  OE                GND
*/
const int s0 = 8;
const int s1 = 9;
const int s2 = 12;
const int s3 = 11;
const int out = 10;
// LED pins connected to Arduino
int redLed = 2;
int greenLed = 3;
int blueLed = 4;
// Variables
int red = 0;
int green = 0;
int blue = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}

void loop()
{
  color();
  //  Serial.print("R Intensity:");
  Serial.print(red);
  Serial.print(" ");
  //  Serial.print(" G Intensity: ");
  Serial.print(green);
  Serial.print(" ");
  //  Serial.print(" B Intensity : ");
  Serial.println(blue);
  //  Serial.print(" ");
  //    Serial.println();
  delay(10);

  //  if (red < blue && red < green && red < 20)
  //  {
  //   Serial.println(" - (Red Color)");
  //   digitalWrite(redLed, HIGH); // Turn RED LED ON
  //   digitalWrite(greenLed, LOW);
  //   digitalWrite(blueLed, LOW);
  //  }
  //
  //  else if (blue < red && blue < green)
  //  {
  //   Serial.println(" - (Blue Color)");
  //   digitalWrite(redLed, LOW);
  //   digitalWrite(greenLed, LOW);
  //   digitalWrite(blueLed, HIGH); // Turn BLUE LED ON
  //  }
  //
  //  else if (green < red && green < blue)
  //  {
  //   Serial.println(" - (Green Color)");
  //   digitalWrite(redLed, LOW);
  //   digitalWrite(greenLed, HIGH); // Turn GREEN LED ON
  //   digitalWrite(blueLed, LOW);
  //  }
  //  else{
  //  Serial.println();
  //  }
  //  delay(300);
  //  digitalWrite(redLed, LOW);
  //  digitalWrite(greenLed, LOW);
  //  digitalWrite(blueLed, LOW);
}

void color()
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
