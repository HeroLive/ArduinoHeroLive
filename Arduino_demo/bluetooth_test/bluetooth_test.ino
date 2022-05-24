#define relay 13

int state;
void setup() {
  pinMode(relay, OUTPUT); 
  digitalWrite(relay, HIGH);
  Serial.begin(9600);
}

void loop() {
 if(Serial.available()>0){
  state= Serial.read();
   
  if(state=='0'){
    digitalWrite(relay, LOW);
    Serial.println("Tat thiet bi");
  }
  else if(state=='1'){
    digitalWrite(relay, HIGH);
    Serial.println("Bat thiêt bi");
  }
}
}  
