//motor A
#define enA 10
#define in1 9
#define in2 8
//motor B
#define in3 7
#define in4 6
#define enB 5

String Str;

void setup() {
 pinMode(enA,OUTPUT);
 pinMode(in1,OUTPUT);
 pinMode(in2,OUTPUT);
 pinMode(enB,OUTPUT);
 pinMode(in3,OUTPUT);
 pinMode(in4,OUTPUT);
 Serial.begin(9600);
}

void loop() {
//  Serial.println("Car to go");
//  while(Serial.available()==0){
//    mstop();
//  }
//  Str = Serial.readString();
//  if(Str=="F"){
//    forward();
//    delay(1000);
//  }
//  if(Str=="B"){
//    backward();
//    delay(1000);
//  }
//  if(Str=="L"){
//    left();
//    delay(1000);
//  }
//  if(Str=="R"){
//    right();
//    delay(1000);
//  }
//  if(Str=="S"){
//    mstop();
//  }
  forward();
  delay(1000);
}

void forward(){
  //motor A
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA,50);
  //motor B
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enB,50);
}

void backward(){
  //motor A
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(enA,100);
  //motor B
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enB,100);
}

void mstop(){
  //motor A
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  //motor B
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void left(){
  //motor A
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  //motor B
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enB,60);
}

void right(){
  //motor A
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA,60);
  //motor B
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  
}
