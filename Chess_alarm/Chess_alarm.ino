#include <TM1637Display.h>

//encoder
#define outputA 2
#define outputB 3
#define enSw 4

// 4-digital display pins (Digital Pins)
#define CLKA 5
#define DIOA 6
#define CLKB 7
#define DIOB 8
TM1637Display displayA(CLKA, DIOA);
TM1637Display displayB(CLKB, DIOB);


//button, led, buzzer
#define btA 9
#define btB 10
#define buzz 11

//
uint8_t segOff[]={false,false};
//
 int timerSet = 0; 
 int aState;
 int aLastState;
 int modeState=0;  
 int modeStateLast=0;

unsigned long minSetA = 5;
unsigned long secSetA = 0;
unsigned long minSetB = 5;
unsigned long secSetB = 0;
unsigned long timeO = 0;
unsigned long timeA = 0;
unsigned long timeB = 0;
int enSwStatus=1;
int enSwStatusLast=1;
int btAStatus=1;
int btAStatusLast=1;
int btBStatus=1;
int btBStatusLast=1;

void setup() {
 Serial.begin(9600);
 
 displayA.setBrightness(4);
 displayA.clear();
 displayB.setBrightness(4);
 displayB.clear();
 delay(1000);
 
 pinMode(btA,INPUT_PULLUP);
 pinMode(btB,INPUT_PULLUP);
 pinMode(buzz,OUTPUT);

 pinMode (outputA,INPUT);
 pinMode (outputB,INPUT);
 pinMode(enSw,INPUT_PULLUP);
 aLastState = digitalRead(outputA);  
 digitalWrite(buzz,LOW); 

}

void loop() {
  Serial.println(modeState);
  switch(modeState){
    
    case 0:
      digitalWrite(buzz,LOW);
      displayA.showNumberDecEx(minSetA*100 + secSetA, 0b01000000, true, 4, 0);
      displayB.showNumberDecEx(minSetB*100 + secSetB, 0b01000000, true, 4, 0);
      enSwStatus=digitalRead(enSw);
      btAStatus=digitalRead(btA);
      btBStatus=digitalRead(btB);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=1;
      }
      if(btAStatus==0&&btAStatusLast==1){
        modeState=12;
        timeO= millis();
        timeA=0;
        timeB=0;
      }
      if(btBStatus==0&&btBStatusLast==1){
        modeState=11;
        timeO= millis();
        timeA=0;
        timeB=0;
      }
      btAStatusLast=btAStatus;
      btBStatusLast=btBStatus;
      enSwStatusLast=enSwStatus;
      break;
      
    case 1:
      enSwStatus=digitalRead(enSw);
      minSetA=setNum(minSetA);
      displayBlink("A",0);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=2;
      }
      enSwStatusLast=enSwStatus;
      break;
    case 2:
      enSwStatus=digitalRead(enSw);
      secSetA=setNum(secSetA);
      displayBlink("A",2);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=3;
        displayBlink("Null",-1);
      }
      enSwStatusLast=enSwStatus;
      break;
    case 3:
      enSwStatus=digitalRead(enSw);
      minSetB=setNum(minSetB);
      displayBlink("B",0);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=4;
      }
      enSwStatusLast=enSwStatus;
      break;
    case 4:
      enSwStatus=digitalRead(enSw);
      secSetB=setNum(secSetB);
      displayBlink("B",2);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=0;
      }
      enSwStatusLast=enSwStatus;
      break;
      
    case 11:
      timeA = millis()-timeO-timeB;
      if(timeOut()==1){
        modeState=13;
      }
      displayA.showNumberDecEx(timeDisplay(timeA,"A"), 0b01000000, true, 4, 0);
      displayB.showNumberDecEx(timeDisplay(timeB,"B"), 0b01000000, true, 4, 0);
      btAStatus=digitalRead(btA);
      if(btAStatus==0&&btAStatusLast==1){
        modeState=12;
      }
      btAStatusLast=btAStatus;
      enSwStatus=digitalRead(enSw);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=0;
      }
      enSwStatusLast=enSwStatus;
      break;
      
    case 12:
      timeB = millis()-timeO-timeA;
      if(timeOut()==1){
        modeState=13;
      }
      displayA.showNumberDecEx(timeDisplay(timeA,"A"), 0b01000000, true, 4, 0);
      displayB.showNumberDecEx(timeDisplay(timeB,"B"), 0b01000000, true, 4, 0);
      btBStatus=digitalRead(btB);
      if(btBStatus==0&&btBStatusLast==1){
        modeState=11;
      }
      btBStatusLast=btBStatus;
      enSwStatus=digitalRead(enSw);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=0;
      }
      enSwStatusLast=enSwStatus;
      break;
    case 13:
      analogWrite(buzz,255);
      delay(100);
      analogWrite(buzz,0);
      delay(100);
      enSwStatus=digitalRead(enSw);
      if(enSwStatus==0&&enSwStatusLast==1){
        modeState=0;
      }
      enSwStatusLast=enSwStatus;
      break;
    
    default:
 
      break;
  }
}

int setNum(int num) { 
   aState = digitalRead(outputA);
   if (aState != aLastState){     
     if (digitalRead(outputB) != aState) { 
       num ++;
     } else {
       if(num>0){
         num --;
       }
     }
   } 
   aLastState = aState;
   return num;
 }
 void displayBlink(String player, int pos){
  if(player=="A"){
     if ((millis()*5/1000)%2==0){
        displayA.setSegments(segOff, 2, pos);
     } else displayA.showNumberDecEx(minSetA*100 + secSetA, 0b01000000, true, 4, 0);
  } else if  (player=="B"){
     if ((millis()*5/1000)%2==0){
        displayB.setSegments(segOff, 2, pos);
     } else displayB.showNumberDecEx(minSetB*100 + secSetB, 0b01000000, true, 4, 0);
  } else {
    displayA.showNumberDecEx(minSetA*100 + secSetA, 0b01000000, true, 4, 0);
    displayB.showNumberDecEx(minSetB*100 + secSetB, 0b01000000, true, 4, 0);
  }
 }

 long timeDisplay(long timeRun, String player){
    long min_t=0;
    long sec_t=0;
    if (player=="A"){
        min_t= (minSetA*60+secSetA-timeRun/1000)/60;
        sec_t = (minSetA*60+secSetA-timeRun/1000)%60;
    } else if (player=="B"){
      min_t= (minSetB*60+secSetB-timeRun/1000)/60;
      sec_t= (minSetB*60+secSetB-timeRun/1000)%60;
    }
   
  return min_t*100 + sec_t;
 }

 int timeOut(){
  int isTimeOut=0;
  if (timeA>(minSetA*60+secSetA)*1000){
    isTimeOut=1;
  } else if (timeB>(minSetB*60+secSetB)*1000){
    isTimeOut=1;
  } 
  return isTimeOut;
 }
