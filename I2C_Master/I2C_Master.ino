#include <Wire.h>
#define address 0x05    //slave


void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  sendData(1);
  getData();
}

void sendData(int num){
  Wire.beginTransmission(0x05); // transmit to device 0x05
  //Wire.write("x is ");          // sends five bytes
  Serial.print("Sent: ");
  Serial.println(num);
  Wire.write(num);                // sends one byte  
  Wire.endTransmission();       // stop transmitting  
  delay(100);
}

void getData(){ 
  Wire.requestFrom(0x05,16);

  while(Wire.available()){
    byte data = Wire.read();
    Serial.print("Got: ");
    Serial.println(data);  
  }

  delay(100);
}
