// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

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
