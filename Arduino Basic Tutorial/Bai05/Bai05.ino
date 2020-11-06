int led = 9;
int bright = 255; //0 -- 255
void setup() {
  // put your setup code here, to run once:
pinMode(led,OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(led,bright);
}
