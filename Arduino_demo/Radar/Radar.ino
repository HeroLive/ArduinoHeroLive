// Includes the Servo library
#include <Servo.h>.

// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;

// Variables for the duration and the distance
long duration;
float distance;
float detectedDistance = 10; //cm
boolean servoDir = true;  //false: angle down, true: angle up
int servoPos = 15;

Servo myServo; // Creates a servo object for controlling the servo motor


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(12); // Defines on which pin is the servo motor attached
  myServo.write(servoPos);
}


void loop() {
  // rotates the servo motor from 15 to 165 degrees
  if (servoPos <= 15) {
    servoDir = true;
    delay(100);
  } else if (servoPos >= 165) {
    servoDir = false;
    delay(100);
  }
  servoPos = servoDir ? servoPos + 1 : servoPos - 1;
  myServo.write(servoPos);
  delay(30);
  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree

  Serial.print(servoPos);
  Serial.print("\t");
  Serial.print(distance);
  Serial.println();

  while (distance < detectedDistance) {
    distance = calculateDistance();
    Serial.print("Detected Object at ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
// Function for calculating the distance measured by the Ultrasonic sensor
float calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;
  return distance;
}
