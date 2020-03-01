#include <Wire.h>

#define addresse 0x09 //Teensy Address
int zahl = 0;

//--define led pins
int ledrow1 = 30;
int ledrow2 = 31;
int ledrow3 = 32;
int ledrow4 = 33;
int ledrow5 = 34;
int ledstrip = 36;

//--define motor pins
const int m_a1 = 42;    //motor A signal 1
const int m_a2 = 43;    //motor A signal 2
const int m_a  = 3;    //motor A speed
const int m_b1 = 44;    //motor B signal 1
const int m_b2 = 45;    //motor B signal 2
const int m_b  = 2;    //motor B speed

//--define second h-brücke
const int ledp = 40;
const int ledn = 41;
const int bright = 4;   //brightness


void setup() {
  pinMode(13, OUTPUT);

  //--define led outputs
  pinMode(ledrow1,OUTPUT);
  pinMode(ledrow2,OUTPUT);
  pinMode(ledrow3,OUTPUT);
  pinMode(ledrow4,OUTPUT);
  pinMode(ledrow5,OUTPUT);
  pinMode(ledstrip,OUTPUT);
  //--define motor outputs
  pinMode(m_a1, OUTPUT);  
  pinMode(m_a2, OUTPUT);  
  pinMode(m_a, OUTPUT);
  pinMode(m_b1, OUTPUT);  
  pinMode(m_b2, OUTPUT);  
  pinMode(m_b, OUTPUT);  
  //--define second h-brücke
  pinMode(ledp,OUTPUT);
  pinMode(ledn,OUTPUT);
  pinMode(bright, OUTPUT);
  
  Serial.begin(9600);
  
  bootup_animation();
  Serial.println("Bereit");
}

void loop() {
  ledLeiste(255);
//  drive(getData());  
  drive(45);
  delay(100);
}

void drive(int a){
  ledRowControl(a);
  if(a != 180){
    long dir = getDir(a);
    Serial.print("dir: ");
    Serial.println(dir,BIN);
    byte s2 = dir & 255;
    dir = dir >> 8;
    byte s1 = dir & 255;
    dir = dir >> 8;
    byte a = dir;
    turn(a,s1,s2);
    Serial.print("a: ");
    Serial.print(a);
    Serial.print(", s1: ");
    Serial.print(s1);
    Serial.print(", s2: ");
    Serial.print(s2);
    Serial.println();
  }
}

long getDir(int a){ 
  byte b1,b2,b3;              //motorOn, SpeedB, SpeedA
  b1 = 0;                           
  b2 = 255;
  b3 = 255;
  
  if(a < 90)                  //links
    b2 = constrain(byte(255 - (a*a * 51 / 490)),0,255);
  else
    b3 = constrain(byte(255 - (a*a * 51 / 490)),0,255);

  b1 = (b2 > 0)? (b3 > 0)? 3:2 : (b3 > 0)? 1:0;     //11  , 10 , 01 , 0 --> Motor Turning (0 = back ; 1 = forward)
  long l = (b1 << 8) | b2;
  a =  a << 8 | b3;
  return l;
}

void turn(byte dir, byte sb, byte sa){
  int a1,a2,b1,b2;
  switch(dir){                                          // B   |  A
    case 0: b1 = HIGH; b2 = LOW; a1 = LOW; a2 = HIGH;   //back | back
    case 1: b1 = HIGH; b2 = LOW; a1 = HIGH; a2 = LOW;   //back | forw
    case 2: b1 = LOW; b2 = HIGH; a1 = LOW; a2 = HIGH;   //forw | back
    case 3: b1 = LOW; b2 = HIGH; a1 = HIGH; a2 = LOW;   //forw | forw
  }
  
  digitalWrite(m_a1, a1);  
  digitalWrite(m_a2, a2);         // Motor A dreht.
  analogWrite(m_a, sa);           // Motor A SA% Leistung. 
  digitalWrite(m_b1, b1);      
  digitalWrite(m_b2, b2);         // Motor B dreht.
  analogWrite(m_b, sb);           // Motor B SB% Leistung.     
}

//--I2C------------------------------
byte getData(){ 
  Wire.requestFrom(addresse,1);
  byte data;
  while(Wire.available()){
    data = Wire.read();
    Serial.print("Got: ");
    Serial.println(data);  
  }
  delay(100);
  return data;
}

//--Led-Leiste-----------------------
void ledLeiste(int b){
  digitalWrite(ledp,HIGH);
  digitalWrite(ledn,LOW);
  analogWrite(bright,b);  
}


//--LED Row Controll Void------------
void bootup_animation(){
  for(int i = 0; i < 11; i++){
    ledRowControl(i,false);  
  }
}

void ledRowControl(int angle){
  ledRowControl(angle,true);  
}

void ledRowControl(int angle, bool umrechnen){
  //       1
  //   2       3
  // 4           5
  //6             7
  //        
  //       10
  int led = (umrechnen)? angleToLed(angle):angle;
  
  switch(led){
    case 0: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 1: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, HIGH);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 2: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 3: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, LOW); break;
    case 4: digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 5: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, HIGH); break;
    case 6: digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 7: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, HIGH); break;
    case 8: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, HIGH);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, LOW); break;
    case 9: digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, HIGH); break;
    case 10:digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
  }
}

int angleToLed(int a){
  return a;   //TEMP
}

//---------------OLD STUFF---------------------
















void empfangeDaten(int byteCount) {
  while (Wire.available()) {
    zahl = Wire.read();
    Serial.print("Daten erhalten: ");
    Serial.println(zahl);

    if (zahl == 1) {
        digitalWrite(13, HIGH);
    } else {
        digitalWrite(13, LOW);
    }

    ledRowControl(zahl);
    drive(zahl,0);
  }
}

void sendeDaten() {
  Wire.write(zahl);
}


void drive(int dir, int s){    //direction, speed    

  //        /\
  //      B    A
  //
  //         1
  //      2      3
  //    4         5
  //  6             7
  //         8
  //        10 (180°)
  
  if(dir == 0){               //STOP
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, LOW);       // Motor A dreht jetzt nicht.
    digitalWrite(m_b1, LOW);     
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt nicht.
  }else if(dir == 1){         //AHEAD y
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);       // Motor A dreht jetzt vorwärts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung. 
  }else if(dir == 2){         //LEFT SMALL y
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);        // Motor A dreht jetzt vorwärts.
    analogWrite(m_a, s);            // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);       // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s*0.5);        // Motor B 50% Leistung. 
  }else if(dir == 3){         //RIGHT SMALL y
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);        // Motor A dreht jetzt vorwärts.
    analogWrite(m_a, s*0.5);        // Motor A 50% Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);       // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);            // Motor B 100% Leistung. 
  }else if(dir == 4){         //LEFT MIDDLE y
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);      // Motor A dreht jetzt vorwarts.
    analogWrite(m_a, s*0.25);          // Motor A 100% Leistung. 
    digitalWrite(m_b1, HIGH);      
    digitalWrite(m_b2, LOW);      // Motor B dreht jetzt rückwärts.
    analogWrite(m_b, s);      // Motor B 50% Leistung.   
  }else if(dir == 5){         //RIGHT MIDDLE y
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt rückwarts.
    analogWrite(m_a, s);           // Motor A 50% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);      // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s*0.25);           // Motor B 100% Leistung.   
  }else if(dir == 6){         //LEFT HIGH y
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);       // Motor A dreht jetzt vorwärts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, HIGH);      
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt rückwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.     
  }else if(dir == 7){         //RIGHT HIGH y
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt rückwarts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, LOW);      
    digitalWrite(m_b2, HIGH);       // Motor B dreht jetzt vorwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.     
  }else if(dir == 8){         //BACK y
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);      // Motor A dreht jetzt rückwärts.
    analogWrite(m_a, s);           // Motor A 100% Leistung. 
    digitalWrite(m_b1, HIGH);  
    digitalWrite(m_b2, LOW);       // Motor B dreht jetzt rückwärts.
    analogWrite(m_b, s);           // Motor B 100% Leistung.     
  }
}
