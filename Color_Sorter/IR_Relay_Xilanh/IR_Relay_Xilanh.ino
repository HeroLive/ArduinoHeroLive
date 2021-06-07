#define R_IR 2
#define G_IR 3
#define R_Xilanh 11
#define G_Xilanh 12
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
}

void loop()
{
  Serial.print(digitalRead(R_IR));
  Serial.print(" ");
  Serial.println(digitalRead(G_IR));
  if (digitalRead(R_IR) == 0) {
    while (digitalRead(R_IR) == 0){
      Serial.println("Red_IR");
    }
    digitalWrite(R_Xilanh, HIGH);
    delay(400);
    digitalWrite(R_Xilanh,LOW);
  }
  if (digitalRead(G_IR) == 0) {
    while (digitalRead(G_IR) == 0){
      Serial.println("Green_IR");
    }
    digitalWrite(G_Xilanh, HIGH);
    delay(400);
    digitalWrite(G_Xilanh,LOW);
  }
  delay(100);
}
