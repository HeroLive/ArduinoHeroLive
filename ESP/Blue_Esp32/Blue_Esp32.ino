/*Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * Thanks to Neil Kolbans for his efoorts in adding the support to Arduino IDE
 * Turotial on: www.circuitdigest.com 
 */

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 2;
String s;

void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
}
int in = 0;
void loop() {
  delay(1000);
  in++;
  ESP_BT.println(in);
  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    s = ESP_BT.readString(); //Read what we recevive 
    Serial.print("Received:"); Serial.println(s);
    ESP_BT.println(s);
    if (incoming == 49)
        {
        digitalWrite(LED_BUILTIN, HIGH);
        ESP_BT.println("LED turned ON");
        }
        
    if (incoming == 48)
        {
        digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("LED turned OFF");
        }     
  }
  delay(20);
}
