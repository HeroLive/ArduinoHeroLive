#define IR01 2
#define R_Xilanh 11
int IR01_status = 1;
void setup()
{
  Serial.begin(9600);
//  pinMode(IR_01, INPUT);
  pinMode(R_Xilanh, OUTPUT);
  digitalWrite(R_Xilanh, LOW);
  delay(100);
}

void loop()
{
  IR01_status = digitalRead(IR01);
  Serial.println(IR01_status);
  if (IR01_status == 0) {
    while (IR01_status == 0){
      IR01_status = digitalRead(IR01);
      Serial.println("IR01");
    }
    digitalWrite(R_Xilanh, HIGH);
    delay(400);
    digitalWrite(R_Xilanh,LOW);
  }
  delay(100);
}
