#define R_Xilanh 11
#define G_Xilanh 12
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(R_Xilanh,OUTPUT); 
  pinMode(G_Xilanh,OUTPUT); 
  digitalWrite(R_Xilanh,LOW); // 0V
  digitalWrite(G_Xilanh,LOW); // 0V
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(R_Xilanh,HIGH); // 0V
  delay(2000);
  digitalWrite(R_Xilanh,LOW); // 0V
  
  digitalWrite(G_Xilanh,HIGH); // 0V
  delay(2000);  
  digitalWrite(G_Xilanh,LOW); // 0V
  delay(5000);
}
