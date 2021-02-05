// Run a A4998 Stepstick from an Arduino UNO.
int x = 0; 
int n=0;
int en = 8 ;
int dirX = 5 ;
int stepX = 2 ;

void setup() 
{
  Serial.begin(9600);
  pinMode(en,OUTPUT); // Enable
  pinMode(stepX,OUTPUT); // Step
  pinMode(dirX,OUTPUT); // Dir
  digitalWrite(en,LOW); // Set Enable low
}

void loop() {
  Serial.print("Quay bao nhieu vong?: ");
  while(Serial.available()==0){
    }
  n = Serial.parseInt();
  Serial.print(n);
  x = n*200;
  digitalWrite(dirX,LOW);
  for (int i = 1 ; i<= x ; i=i+1){
    digitalWrite(stepX,HIGH);
    delay(1);
    digitalWrite(stepX,LOW);
    delay(1);
  }
  delay(2000);
}
