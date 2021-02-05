// Run a A4998 Stepstick from an Arduino UNO.
int x = 0; 
int n=0;
int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;
int endPinX = 3;
bool dirX = LOW;
int lastEndX = 1;

void setup() 
{
  Serial.begin(9600);
  pinMode(en,OUTPUT); // Enable
  pinMode(stepPinX,OUTPUT); // Step
  pinMode(dirPinX,OUTPUT); // Dir
  pinMode(endPinX,INPUT_PULLUP);
  digitalWrite(en,LOW); // Set Enable low
  digitalWrite(dirPinX,dirX);
}

void loop() {
//  Serial.println(digitalRead(endPinX));
  
  if (digitalRead(endPinX)!= lastEndX){
    if(digitalRead(endPinX)==0){
       lastEndX = 0;
       dirX = !dirX; 
       digitalWrite(dirPinX,dirX);
    } else {
      lastEndX = 1;
    }
  }
    digitalWrite(stepPinX,HIGH);
    delay(1);
    digitalWrite(stepPinX,LOW);
    delay(10);
}
