int en = 8 ;
int dirPinX = 5 ;
int stepPinX = 2 ;
int dirPinZ = 7 ;
int stepPinZ = 4 ;

int pulseMax = 0; //print to monitor
int pulseMin = 0; //print to monitor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(stepPinZ, OUTPUT); // Step
  pinMode(dirPinZ, OUTPUT); // Dir
  digitalWrite(dirPinX, LOW); // Set Enable low
  digitalWrite(dirPinZ, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveXZ(800, stepPinX, 200, stepPinZ);
  delay(5000);
  //  Serial.println("==========================");
}

void moveXZ(long nStepX, int stepPinX, long nStepZ, int stepPinZ) {

  float nStepMax = nStepX;
  float nStepMin = nStepZ;
  int stepPinMax = stepPinX;
  int stepPinMin = stepPinZ;

  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0;

  int pulseMax = 0; //print to monitor
  int pulseMin = 0; //print to monitor

  if (nStepZ > nStepX) {
    nStepMax = nStepZ;
    nStepMin = nStepX;
    stepPinMax = stepPinZ;
    stepPinMin = stepPinX;
  }

  ratio_max_min = nStepMax / nStepMin;

  for (int i = 1 ; i <= nStepMax ; i = i + 1) {
    current_axis_min = i / ratio_max_min;

    if (current_axis_min - steps_axis_min >= 1) {
      digitalWrite(stepPinMin, HIGH);
      pulseMin = 1;
      steps_axis_min++;
    }

    digitalWrite(stepPinMax, HIGH);
    pulseMax = 1;
    Serial.print(pulseMax);
    Serial.print(" ");
    Serial.println(pulseMin);
    delay(1);

    digitalWrite(stepPinMax, LOW);
    digitalWrite(stepPinMin, LOW);
    pulseMax = 0;
    pulseMin = 0;
    Serial.print(pulseMax);
    Serial.print(" ");
    Serial.println(pulseMin);
    delay(1);
  }
}
