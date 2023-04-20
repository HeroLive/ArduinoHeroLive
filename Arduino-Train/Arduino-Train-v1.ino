int Din[16] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52 };
int Dout[16] = { 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53 };
long _tdelay = 1000;
long time[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool status[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 16; i++) {
    pinMode(Din[i], INPUT_PULLUP);
    pinMode(Dout[i], OUTPUT);
    digitalWrite(Dout[i], status[i]);
  }
}

void loop() {
  for (int i = 0; i < 16; i++) {
    if (digitalRead(Din[i])==0) {
      status[i] = 0;
      time[i] = millis();
    }
    if (millis() - time[i] > _tdelay) {
      status[i] = 1;
    }
    digitalWrite(Dout[i], status[i]);
    Serial.print(status[i]);
    if(i==15){
      Serial.println();
    } else {
      Serial.print(" ");
    }
  }
}
