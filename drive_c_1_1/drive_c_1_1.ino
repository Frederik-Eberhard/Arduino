#include <Wire.h>

#define adresse 0x05
#define sender 2
#define empfaenger 3

const int m_a1 = 30;    //motor A signal 1
const int m_a2 = 31;    //motor A signal 2
const int m_a  = 10;    //motor A speed
const int m_b1 = 32;    //motor B signal 1
const int m_b2 = 33;    //motor B signal 2
const int m_b  = 11;    //motor B speed

const int led1 = 19;    //com_led

int zahl = 0;           //com_data
int messung, abstand;
long preDistSen;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(led1, OUTPUT);
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

  pinMode(sender, OUTPUT);                    // Digital 2 ist ein Sender (PWM).
  pinMode(empfaenger, INPUT);                 // Digiatl 3 ist ein Empfänger. 
  delay(10);
  preDistSen = millis();

}
void loop() {
  drive(1,255);
  delay(20);
  if(millis() > preDistSen + 50) {
    preDistSen = millis();
    if(ultraschall() < 4){
    //flasche();
    Serial.print("Flasche");
    }
  }
}

void empfangeDaten(int byteCount) {
  while (Wire.available()) {
    zahl = Wire.read();
    Serial.print("Daten erhalten: ");
    Serial.println(zahl);


    if(zahl == -1){   //COM ERROR    doesnt work!
      digitalWrite(led1,HIGH);
    }
    else {
      digitalWrite(led1,LOW);

      if (zahl < 5) {  //0-4
        if(zahl == 0){
          digitalWrite(13, HIGH);
        }
        else{
          digitalWrite(13, LOW);   
        }
        drive(zahl,255);
        delay(200);
      }
      else{ 
        if(zahl == 5) {  //180° Drehung
          drive(2,255);
          delay(2500);
        }
      }
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


int ultraschall(){
  digitalWrite(sender, HIGH);                 // Ein Ultraschallsignal wird gesendet.
  delayMicroseconds(100);                     // Er macht es 100ms=0,1s.
  digitalWrite(sender, LOW);                  // Das Ultraschallsignal wird beendet.
  
  messung = pulseIn(empfaenger, HIGH,14*57.7);        // Laufzeitmessung
  abstand = messung/57.7;                     // Umrechnung in cm
  delay(20);
  if(abstand == 0){
    abstand = 20;
  }
  Serial.print(abstand);                      // Ausgabe auf den Monitor
  Serial.println(" cm"); 

  return abstand;
}



void flasche(){
  drive(0,0);
  delay(2000);
  drive(3,255);
  delay(200);
  drive(4,255);
  delay(2000);
  drive(1,255);
  delay(3000);
  drive(2,255);
  delay(2000);
  drive(1,255);
  delay(4000);
  drive(2,255);
  delay(2000);
  drive(1,255);
  delay(3000);
  drive(4,255);
  delay(2000);
}


