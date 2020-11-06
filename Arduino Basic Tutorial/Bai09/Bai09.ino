int led1 = 2;
int led2 = 3;
int time1 = 500;
int time2 = 1000;
int i = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=1;i<=4;i=i+1){
    digitalWrite(led1,HIGH);
    delay(time1);
    digitalWrite(led1,LOW);
    delay(time1);
  }
  

  //led2
  for(i=1;i<=10;i++){
    digitalWrite(led2,HIGH);
    delay(time2);
    digitalWrite(led2,LOW);
    delay(time2);
  }
}
