#include <Wire.h>
#define address 0x08    //slave

byte num = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(address);

  Wire.onReceive(getData);
  Wire.onRequest(sendData);

  Serial.println("Ready");
}

void loop() {

}

void getData(int byteCount) {
  while (Wire.available()) {
    int data = Wire.read();
    Serial.print("Data:");
    Serial.println(data);
    delay(100);
  }
}

void sendData() {
  Serial.print("Sending: ");
  Serial.println(num);
  Wire.write(num);
  num +=1;
  delay(100);
}
