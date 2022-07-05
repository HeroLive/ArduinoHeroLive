//Define for use pins by IC 74hc595 control by EPS32 on MKS DLC32
#define enXYZ 7//QA-15
#define pulX 6 //QB-1
#define dirX 5 //QC-2
#define pulZ 4 //QF-3
#define dirZ 3 //QG-4
#define pulY 2 //QD-5
#define dirY 1 //QE-6
int pin_latch = 17; //I2S_WS
int pin_clock = 16; //I2S_BCK
int pin_data = 21; //I2S_DATA
byte IC74HC = 0;

long stepX = 400; //1 vong
long stepY = 800;  //2 vong
long stepZ = 1200; //3 vong

long _delay = 1000;

void setup() {
  Serial.begin(9600);
  // Set all the pins of 74HC595 as OUTPUT
  pinMode(pin_latch, OUTPUT);
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_data, OUTPUT);

  //disable motor by HIGH
  bitSet(IC74HC, enXYZ);
  updateShiftRegister();
  delay(3000);
}

void loop() {
  runStepper(pulX,dirX,enXYZ,true,stepX);
  delay(500);
  runStepper(pulX,dirX,enXYZ,false,stepX);
  delay(1000);
  
  runStepper(pulY,dirY,enXYZ,true,stepY);
  delay(500);
  runStepper(pulY,dirY,enXYZ,false,stepY);
  delay(1000);
  
  runStepper(pulZ,dirZ,enXYZ,true,stepZ);
  delay(500);
  runStepper(pulZ,dirZ,enXYZ,false,stepZ);
  delay(1000);
}

void runStepper(int pin_pul, int pin_dir, int pin_en, bool dir, long _step) {
  
  //enable motor by LOW
  bitClear(IC74HC, pin_en);
  updateShiftRegister();
  
  //set motor direction
  if (dir)
  {
    bitSet(IC74HC, pin_dir);
    updateShiftRegister();
  } else {
    bitClear(IC74HC, pin_dir);
    updateShiftRegister();
  }

  //generate pulse
  for (int i = 0; i < _step; i++) {
    bitSet(IC74HC, pin_pul);
    updateShiftRegister();
    delayMicroseconds(_delay);
    bitClear(IC74HC, pin_pul);
    updateShiftRegister();
    delayMicroseconds(_delay);
  }

  //disable motor by HIGH
  bitSet(IC74HC, pin_en);
  updateShiftRegister();
}

void updateShiftRegister()
{
  digitalWrite(pin_latch, LOW);
  shiftOut(pin_data, pin_clock, LSBFIRST, IC74HC);
  digitalWrite(pin_latch, HIGH);
}
