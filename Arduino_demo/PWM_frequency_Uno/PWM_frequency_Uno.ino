//void setup() {
//  analogWrite(5,25);
//}
//
//void loop(){
//  
//}


void setup() {
pinMode(5, OUTPUT);
pinMode(6,OUTPUT);
TCCR0A=0;//reset the register
TCCR0B=0;//reset tthe register
TCCR0A=0b11100011;// fast pwm mode
TCCR0B=0b00000010;// prescaler 64
OCR0A=50;//duty cycle for pin 6
OCR0B=50;//duty cycle for pin 5
}
void loop() {
// put your main code here, to run repeatedly
}
