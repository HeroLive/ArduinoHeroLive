String msg = "What do you want led color: ";
String color = "";
int redPin = 2;
int greenPin = 3;
int bluePin = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(msg);
  while(Serial.available()==0){
    
  }
  color = Serial.readString();
  
  if (color == "O"){
    Serial.println("Led off");
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,LOW);
  }
  if (color == "R"){
    Serial.println("red");
    digitalWrite(redPin,HIGH);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,LOW);
  }
  if (color == "G"){
    Serial.println("green");
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,HIGH);
    digitalWrite(bluePin,LOW);
  }
  if (color == "B"){
    Serial.println("blue");
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,HIGH);
  }
}
