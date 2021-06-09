/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int R_min = 126;
int R_max = 135;
int G_min = 196;
int G_max = 203;
int B_min = 113;
int B_max = 122;

void setup() {
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

  // Begins serial communication
  Serial.begin(9600);
}

void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, R_min, R_max,  255, 0);

  // Printing the RED (R) value
  //  Serial.print("R = ");
  //  Serial.print(redColor);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, G_min, G_max, 255, 0);

  // Printing the GREEN (G) value
  //  Serial.print(" G = ");
  //  Serial.print(greenColor);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, B_min, B_max,  255, 0);

  // Printing the BLUE (B) value
  //  Serial.print(" B = ");
  //  Serial.print(blueColor);
  delay(100);

  // Checks the current detected color and prints
  // a message in the serial monitor
  if (redColor > 7000 && greenColor > 7000 && blueColor > 7000) {
    Serial.println("Bang tai");
  } else {
    if (redColor < greenColor && redColor < blueColor) {
      Serial.println(" - RED detected!");
    }
    if (greenColor < redColor && greenColor < blueColor) {
      Serial.println(" - GREEN detected!");
    }
    if (blueColor < redColor && blueColor < greenColor) {
      Serial.println(" - BLUE detected!");
    }
  }

  Serial.print(blueColor);
  Serial.print(" ");
  Serial.print(redColor);
  Serial.print(" ");
  Serial.println(greenColor);
}
