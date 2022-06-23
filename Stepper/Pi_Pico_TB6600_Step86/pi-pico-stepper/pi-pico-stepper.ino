#define DIR1_PIN 18
#define PUL1_PIN 19
#define EN1_PIN 20
#define btn 21

double disPerRound1 = 1.0 ;// only round: 1.0 round, vitme T8-4mm: 4.0 mm, GT2 Pulley 16 Teeth: 16x2 = 32.0 mm
int microStep1 = 8; //1: full step, 2: haft step, ...
double angleStep1 = 1.8; //a step angle of 1.8 degrees with 200 steps per revolution
double stepsPerUnit1 = (1 / disPerRound1) * 360.0 * microStep1 / angleStep1; //steps/round or steps/mm ...

//Cai dat so vong quay va toc do quay
double target = 0.5; //vong
double whatSpeed = 1.5; //vong/giay

long step_delay = 1000L * 1000L / stepsPerUnit1 / whatSpeed;
long step_limit = target * stepsPerUnit1;

long step_count = 0;
bool _dir = LOW;
bool _dir_start = HIGH;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  pinMode(btn, INPUT_PULLUP);

  digitalWrite(EN1_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  if (step_count <= 0) {
    _dir = _dir_start;
    delay(500);
  } else if (step_count >= step_limit) {
    _dir = !_dir_start;
    delay(500);
  }
  
  if (digitalRead(btn) == LOW) {
    digitalWrite(EN1_PIN, HIGH);
    step_count = 0;
  } else {
    digitalWrite(EN1_PIN, LOW);
    step_run(PUL1_PIN, DIR1_PIN, _dir);
  }
}

void step_run(int pin_pul, int pin_dir, bool dir) {

  double _delay = 0.5 * step_delay - 5;
  digitalWrite(pin_dir, dir);

  digitalWrite(pin_pul, HIGH);
  delayMicroseconds(_delay);
  digitalWrite(pin_pul, LOW);
  delayMicroseconds(_delay);

  step_count = dir ? step_count + 1 : step_count - 1;
}
