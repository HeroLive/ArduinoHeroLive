int en = 8 ;
int dirA = 5 ;
int stepPinA = 2 ;
int dirB = 7 ;
int stepPinB = 4 ;

int pulseMax = 0; //print to monitor
int pulseMin = 0; //print to monitor

//input field
float speedScroll = 6  ; // vong/phut
float ratioBA = 0.1;

long pulsePerRound = 200; // so xung/vong

long period =   1000 / (speedScroll / 60 * pulsePerRound); // time bettween 2 pulse
long nPulseA = pulsePerRound;
long nPulseB = pulsePerRound * ratioBA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinA, OUTPUT); // Step
  pinMode(dirA, OUTPUT); // Dir
  pinMode(stepPinB, OUTPUT); // Step
  pinMode(dirB, OUTPUT); // Dir
  digitalWrite(dirA, LOW); // Set Enable low
  digitalWrite(dirB, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.println("==========================");
    moveAB(nPulseA, stepPinA, nPulseB, stepPinB);
}

void moveAB(long nStepA, int pinA, long nStepB, int pinB) {

  float steps_max = nStepA;
  float steps_min = nStepB;
  int pin_max = pinA;
  int pin_min = pinB;

  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0;

  if (nStepB > nStepA) {
    steps_max = nStepB;
    steps_min = nStepA;
    pin_max = pinB;
    pin_min = pinA;
  }

  ratio_max_min = steps_max / steps_min;

  //  Serial.print(steps_max);
  //  Serial.print(" -- ");
  //  Serial.print(steps_min);
  //  Serial.print(" -- ");
  //  Serial.println(ratio_max_min);

  for (int i = 1 ; i <= steps_max ; i = i + 1) {
    current_axis_min = i / ratio_max_min;

    if (current_axis_min - steps_axis_min >= 1) {
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
    delay(10);

    digitalWrite(pin_max, LOW);
    digitalWrite(pin_min, LOW);
    pulseMax = 0;
    pulseMin = 0;
    Serial.print(pulseMax);
    Serial.print(" ");
    Serial.println(pulseMin);
    delay(period);
  }
}
