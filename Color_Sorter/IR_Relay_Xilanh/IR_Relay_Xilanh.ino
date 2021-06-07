#define R_IR 2
#define G_IR 3
#define R_Xilanh 11
#define G_Xilanh 12
int R_IR_status = 1;
int G_IR_status = 1;
void setup()
{
  Serial.begin(9600);z
//  pinMode(IR_01, INPUT);
  pinMode(R_Xilanh, OUTPUT);
  pinMode(G_Xilanh, OUTPUT);
  digitalWrite(R_Xilanh, LOW);
  digitalWrite(G_Xilanh, LOW);
  delay(100);
}

void loop()
{
  R_IR_status = digitalRead(R_IR);
  G_IR_status = digitalRead(G_IR);
  Serial.println(R_IR_status);
  if (R_IR_status == 0) {
    while (R_IR_status == 0){
      R_IR_status = digitalRead(R_IR);
      Serial.println("Red_IR");
    }
    digitalWrite(R_Xilanh, HIGH);
    delay(400);
    digitalWrite(R_Xilanh,LOW);
  }
  if (G_IR_status == 0) {
    while (G_IR_status == 0){
      G_IR_status = digitalRead(G_IR);
      Serial.println("Green_IR");
    }
    digitalWrite(G_Xilanh, HIGH);
    delay(400);
    digitalWrite(G_Xilanh,LOW);
  }
  delay(100);
}
