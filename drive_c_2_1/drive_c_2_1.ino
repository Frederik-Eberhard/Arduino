#include <Wire.h>
#define adresse 0x05

const int m_a1 = 42;    //motor A signal 1
const int m_a2 = 43;    //motor A signal 2
const int m_a  = 10;    //motor A speed
const int m_b1 = 44;    //motor B signal 1
const int m_b2 = 45;    //motor B signal 2
const int m_b  = 11;    //motor B speed
const int m_c1 = 40;    //motor Cam signal 1
const int m_c2 = 41;    //motor Cam signal 2
const int m_c  = 9;     //motor Cam speed

const int ledc = 19;    //com_led


int com_data = 0;           //com_data


void setup()
{
  Serial.begin(9600); 
  //--motoren--
  pinMode(m_a1, OUTPUT);  
  pinMode(m_a2, OUTPUT);  
  pinMode(m_a, OUTPUT);
  pinMode(m_b1, OUTPUT);  
  pinMode(m_b2, OUTPUT);  
  pinMode(m_b, OUTPUT);  
  pinMode(m_c1, OUTPUT);
  pinMode(m_c2, OUTPUT);
  pinMode(m_c, OUTPUT);
  
  //--Pi-Coms--
  pinMode(13, OUTPUT);
  pinMode(ledc, OUTPUT);
  
  Wire.begin(adresse);
  Wire.onReceive(empfangeDaten);
  Wire.onRequest(sendeDaten);

  Serial.println("Bereit");
  delay(10);
}

void loop()
{ 
  drive(1,100);
  char ch = Serial.read();
  int i   = (int)ch - 48;
  cam(i);
}

void cam(int dir){
  //Serial.print("Cam: ");
  //Serial.println(dir);
  if(dir == 0){
    digitalWrite(m_c1, LOW);
    digitalWrite(m_c2, LOW);
  }
  if(dir == 1){
    digitalWrite(m_c1, LOW);
    digitalWrite(m_c2, HIGH);
    analogWrite(m_c, 60);
  }
  if(dir == 2){
    digitalWrite(m_c1, HIGH);
    digitalWrite(m_c2, LOW);
    analogWrite(m_c, 60);
  }  
  delay(dir==2?50:70);    //down is faster than up -> equal now
  digitalWrite(m_c1, LOW);
  digitalWrite(m_c2, LOW);
}

void drive(int dir, int s){    //direction, speed    

  //        /\
  //      A    B
  //
  //         1
  //   2	   3
  // 4		     5
  //6	              7
  //	     10
	
  if(dir == 0){               //STOP
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, LOW);       // Motor A dreht jetzt nicht.
    digitalWrite(m_b1, LOW);     
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt nicht.
  }else if(dir == 1){         //AHEAD
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwärts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung. 
  }else if(dir == 2){         //LEFT SMALL
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s*0.7);       // Motor A 70% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung. 
  }else if(dir == 3){         //RIGHT SMALL
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s*0.7);       // Motor B 70% Leistung. 
  }else if(dir == 4){         //LEFT MIDDLE
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s*0.3);       // Motor A 30% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.   
  }else if(dir == 5){         //RIGHT MIDDLE
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s*0.3);       // Motor B 30% Leistung.   
  }else if(dir == 6){         //LEFT HIGH
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt rückwarts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, HIGH);      
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.     
  }else if(dir == 7){         //RIGHT HIGH 
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, HIGH);      
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt rückwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.     
  }
}

void empfangeDaten(int byteCount) {
  while (Wire.available()) {
    com_data = Wire.read();
    Serial.print("Daten erhalten: ");
    Serial.println(com_data);


    if(com_data == -1){   //COM ERROR    doesnt work!
      digitalWrite(ledc,HIGH);
    }
    else {
      digitalWrite(ledc,LOW);

      if (com_data < 5) {  //0-4
        if(com_data == 0){
          digitalWrite(13, HIGH);
        }
        else{
          digitalWrite(13, LOW);   
        }
        drive(com_data,255);
        delay(200);
      }
      else{ 
        if(com_data == 5) {  //180° Drehung
          drive(2,255);
          delay(2500);
        }
      }
    }
  }
}

void sendeDaten() {
  Wire.write(com_data);
}


