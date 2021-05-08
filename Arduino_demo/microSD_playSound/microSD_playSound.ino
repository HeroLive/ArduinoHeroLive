// CONNECTIONS:
//SD card attached to SPI bus as follows:
//SD MOSI - pin 11
//SD MISO - pin 12
//SD CLK - pin 13
//SD CS - pin 10

#include <SD.h>
#include <TMRpcm.h>           //  also need to include this library...

TMRpcm tmrpcm;   // create an object for use in this sketch

const int SD_ChipSelectPin = 10;

void setup ()
{
  Serial.begin(9600);

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc

  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    return;   // don't do anything more if not
  }
  tmrpcm.volume(1);
  tmrpcm.play("6.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
}

void loop ()
{
//  tmrpcm.play("6.wav");
  
//  delay(3000);
//  tmrpcm.play("5.wav");
//  delay(3000);
}
