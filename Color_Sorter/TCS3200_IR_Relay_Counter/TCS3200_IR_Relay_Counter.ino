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

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
char color = 'N';
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
  digitalWrite(R_Xilanh, LOW);
  digitalWrite(G_Xilanh, LOW);
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

  delay(1000);
}

void loop()
{
  getColor();
  Serial.print(R_cnt);
  Serial.print(" ");
  Serial.print(G_cnt);
  Serial.print(" ");
  Serial.println(B_cnt);
  if (digitalRead(R_IR) == 0) {
    while (digitalRead(R_IR) == 0) {
      Serial.println("Red_IR");
    }
    if (color == 'R') {
      digitalWrite(R_Xilanh, HIGH);
      delay(400);
      digitalWrite(R_Xilanh, LOW);
      R_cnt++;
    }

  }
  if (digitalRead(G_IR) == 0) {
    while (digitalRead(G_IR) == 0) {
      Serial.println("Green_IR");
    }
    if (color == 'G') {
      digitalWrite(G_Xilanh, HIGH);
      delay(400);
      digitalWrite(G_Xilanh, LOW);
      G_cnt++;
    } else if (color == 'B') {
      B_cnt++;
    }
  }
  delay(100);
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
    color = 'N';
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
  }
}
