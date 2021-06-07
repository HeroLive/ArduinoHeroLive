#define IR_01 2
#define IR_01 3

void setup() 
{
  Serial.begin(9600);
  pinMode(IR_01,INPUT_PULLUP);
}

void loop() 
{
  Serial.println(digitalRead(IR_01));
  delay(100);
}
