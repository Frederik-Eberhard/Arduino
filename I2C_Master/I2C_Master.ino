#include <Wire.h>
#define address 0x08    //slave
 
int n = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  sendData(n++);
  getData();
}

void sendData(int num){
  Wire.beginTransmission(address); // transmit to device 0x05
  //Wire.write("x is ");          // sends five bytes
  Serial.print("Sent: ");
  Serial.println(num);
  Wire.write(num);                // sends one byte  
  Wire.endTransmission();       // stop transmitting  
  delay(100);
}

void getData(){ 
  Wire.requestFrom(address,16);

  while(Wire.available()){
    byte data = Wire.read();
    Serial.print("Got: ");
    Serial.println(data);  
  }

  delay(100);
}
