int en = 8 ;
int dirX = 5 ;
int stepX = 2 ;
int dirZ = 7 ;
int stepZ = 4 ;

int stepsX = 10;
int stepsZ = 5;
int pulseMax = 0; //print to monitor
int pulseMin = 0; //print to monitor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en,OUTPUT); // Enable
  pinMode(stepX,OUTPUT); // Step
  pinMode(dirX,OUTPUT); // Dir
  pinMode(stepZ,OUTPUT); // Step
  pinMode(dirZ,OUTPUT); // Dir
  digitalWrite(dirX,LOW); // Set Enable low
  digitalWrite(dirZ,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveXZ(400,stepX,200,stepZ);
  delay(5000);
//  Serial.println("==========================");
}

void moveXZ(long stepsX,int pinX,long stepsZ,int pinZ){
  
  float steps_max = stepsX;
  float steps_min = stepsZ;  
  int pin_max = pinX;
  int pin_min = pinZ;
  
  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0; 

  if (stepsZ > stepsX){
    steps_max = stepsZ;
    steps_min = stepsX;
    pin_max = pinZ;
    pin_min = pinX;
  }

  ratio_max_min = steps_max/steps_min;

//  Serial.print(steps_max);
//  Serial.print(" -- ");
//  Serial.print(steps_min);
//  Serial.print(" -- ");
//  Serial.println(ratio_max_min);

  for (int i = 1 ; i <= steps_max ; i = i + 1) {
    current_axis_min = i / ratio_max_min;
    
    if (current_axis_min - steps_axis_min >=1){
      digitalWrite(pin_min, HIGH);
      pulseMin = 1;
      steps_axis_min++; 
    }
    
//    Serial.print(current_axis_min);
//    Serial.print(" ");
//    Serial.print(steps_axis_min);
//    Serial.print(" ");
    
    digitalWrite(pin_max, HIGH);
    pulseMax = 1;
    Serial.print(pulseMax);
    Serial.print(" ");
    Serial.println(pulseMin);
    delay(1);
    
    digitalWrite(pin_max, LOW);
    digitalWrite(pin_min, LOW);
    pulseMax = 0;
    pulseMin = 0;
    Serial.print(pulseMax);
    Serial.print(" ");
    Serial.println(pulseMin);
    delay(1);
  }
}
