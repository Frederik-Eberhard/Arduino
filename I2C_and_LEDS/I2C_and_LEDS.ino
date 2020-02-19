#include <Wire.h>

#define adresse 0x05
int zahl = 0;

//--define led pins
int ledrow1 = 30;
int ledrow2 = 31;
int ledrow3 = 32;
int ledrow4 = 33;
int ledrow5 = 34;


void setup() {
  pinMode(13, OUTPUT);

  //--define led outputs
  pinMode(ledrow1,OUTPUT);
  pinMode(ledrow2,OUTPUT);
  pinMode(ledrow3,OUTPUT);
  pinMode(ledrow4,OUTPUT);
  pinMode(ledrow5,OUTPUT);
  
  Serial.begin(9600);
  Wire.begin(adresse);

  Wire.onReceive(empfangeDaten);
  Wire.onRequest(sendeDaten);

  Serial.println("Bereit");
}

void loop() {
  delay(100);
}

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
  }
}

void sendeDaten() {
  Wire.write(zahl);
}


//--LED Row Controll Void
void ledRowControl(int led){
  switch(led){
    case 0: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 1: digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 2: digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 3: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 4: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, HIGH);
            digitalWrite(ledrow3, HIGH);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 5: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, HIGH);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, LOW); break;
    case 6: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, HIGH);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, LOW); break;
    case 7: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, LOW); break;
    case 8: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, HIGH);
            digitalWrite(ledrow5, HIGH); break;
    case 9: digitalWrite(ledrow1, LOW);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, HIGH); break;
    case 10:digitalWrite(ledrow1, HIGH);
            digitalWrite(ledrow2, LOW);
            digitalWrite(ledrow3, LOW);
            digitalWrite(ledrow4, LOW);
            digitalWrite(ledrow5, HIGH); break;
  
  }
}
