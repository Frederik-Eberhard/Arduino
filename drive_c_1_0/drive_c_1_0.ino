#include <Wire.h>

#define adresse 0x05
int zahl = 0;

const int m_a1 = 30;    //motor A signal 1
const int m_a2 = 31;    //motor A signal 2
const int m_a  = 10;    //motor A speed
const int m_b1 = 32;    //motor B signal 1
const int m_b2 = 33;    //motor B signal 2
const int m_b  = 11;    //motor B speed



void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin(adresse);

  Wire.onReceive(empfangeDaten);
  Wire.onRequest(sendeDaten);

  Serial.println("Bereit");

  //--motor-define---

  pinMode(m_a1, OUTPUT);
  pinMode(m_a2, OUTPUT);
  pinMode(m_a, OUTPUT);
  pinMode(m_b1, OUTPUT);
  pinMode(m_b2, OUTPUT);
  pinMode(m_b, OUTPUT);

}
void loop() {
  delay(100);
}

void empfangeDaten(int byteCount) {
  while (Wire.available()) {
    zahl = Wire.read();
    Serial.print("Daten erhalten: ");
    Serial.println(zahl);

    if (zahl < 5) {  //0-4
      if(zahl == 0){
        digitalWrite(13, HIGH);
      }
      drive(zahl,100);
    }
     
     
    
    else {
      digitalWrite(13, LOW);
      drive(1,0);  //stop
    }
  }
}

void sendeDaten() {
  Wire.write(zahl);
}

void drive(int dir, int s){    //direction, speed    

//    /\
//  A    B
//
  if(dir == 0){
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, LOW);  // Motor 1 dreht jetzt nicht.
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, LOW);  // Motor 2 dreht jetzt nicht.
    
  }
  if(dir == 1){
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);  // Motor 1 dreht jetzt vorwärts.
    analogWrite(m_a, s);       // Motor 1 Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);  // Motor 2 dreht jetzt vorwärts.
    analogWrite(m_b, s);       // Motor 2 Leistung. 
  }
  if(dir == 2){
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);   // Motor 1 dreht jetzt vorwärts.
    analogWrite(m_a, s);       // Motor 1 Leistung. 
    digitalWrite(m_b1, HIGH);  
    digitalWrite(m_b2, LOW);  // Motor 2 dreht jetzt rückwärts.
    analogWrite(m_b, s);       // Motor 2 Leistung. 
  }
  if(dir == 3){
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);  // Motor 1 dreht jetzt rückwärts.
    analogWrite(m_a, s);       // Motor 1 Leistung. 
    digitalWrite(m_b1, HIGH);  
    digitalWrite(m_b2, LOW);  // Motor 2 dreht jetzt rückwärts.
    analogWrite(m_b, s);       // Motor 2 Leistung. 
  }
  if(dir == 4){
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);  // Motor 1 dreht jetzt rückwärts.
    analogWrite(m_a, s);       // Motor 1 Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);  // Motor 2 dreht jetzt vorwärts.
    analogWrite(m_b, s);       // Motor 2 Leistung. 
  }
}

