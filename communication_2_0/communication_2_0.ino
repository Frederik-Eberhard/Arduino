/*

Author: Carlo Brokering
05.12.2018

*/


#include "Wire.h"

#define COM_PIN_PI 5      // INPUT PIN FÜR DIE KOMMUNIKATION

long sec = 1;
long count = 0;
unsigned int fps = 0;

unsigned long lastcom = 0;

String in = "";
int inInt[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //    alle daten in der Reihenfolge, wie sie abgesendet wurden
unsigned int inIndex = 0;

void setup() {
  pinMode(COM_PIN_PI, INPUT);
  
  Serial1.begin(115200);
  Serial.begin(115200);

  estPiCon();
}

void loop() {
    // REQUEST DATA
    reqdata();

    if (millis() > lastcom + 500) {     // LAST DATA RECIEVED > 500ms
            
      // !!! SHUT MOTORS OFF

      // DEBUG USB SERIAL
      Serial.print("Lost con to Pi reconnect. mil: ");
      Serial.print(millis());
      Serial.print("  lastcom: ");
      Serial.println(lastcom);


      // TRY TO RECONNECT -> ENDLESS LOOP
      estPiCon();
      Serial.println("Reconnected to Pi!");
      lastcom = millis();
      
    }

    // PROCESS RECIEVED DATA
    
    // DRIVE   
     
    //SHOW DATA ON DISPLAY
}


// KOMMUNIKATIONS METHODEN

void estPiCon() { // ESTIMATE CONNECTION TO RASPBERRY PI ON SERIAL1
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Est. connection to Pi");
  while (Serial1.available() <= 0) {
    delay(200);
    Serial1.println('A');   // send a capital A to Raspberry Pi
    Serial.print(".");
  }
  bool rdy = false;
  int trynmbr = 0;
  while (!rdy) {
    if (Serial1.available() > 0) {
      if (Serial1.read() == 'B') {
        rdy = true; 
      }
    }
    delay(50);
    trynmbr++;
    if(trynmbr > 10) {
      Serial1.println('A');   // send a capital A to Raspberry Pi
    }
  }
  Serial.println();
  Serial.println("Connection to Pi established!");
  lastcom = millis();
  Serial1.println("0");
}

int reqdata() {   // REQUEST DATA VIA SERIAL1
  if (digitalRead(COM_PIN_PI) == HIGH) {
    bool complete = false;
    while (Serial1.available() > 0) {   // SOLANGE DATEN VERFÜGBAR SIND
      int inByte = Serial1.read();      // READ INCOMING BYTE
      if (inByte == ':') {              // ":"  ->  beginning of array
        for (int i = 0; i < 10; i++) {
          inInt[i] = 0;                 // RESET inInt ARRAY
        }
        inIndex = 0;                    // RESET index
      } else if (inByte == '!') {       // "!" ->  end of array
        complete = true;                //ARRAY COMPLETE -> SAVE DATA TO GLOBAL ARRAY
      } else if (isDigit(inByte)) {     // check if char is a digit
        in += (char)inByte;             // add character to string
      } else if (inByte == '\n') {      // number is complete
        inInt[inIndex] = in.toInt();    // write number from String into the array
        inIndex++;                      // rise index by 1
        in = "";                        // reset String to ""
      } else {                          // unknown / error -> reset array and index -> return 0
        Serial.write(inByte);           // output inByte
        Serial.println("com error");
        for (int i = 0; i < 10; i++) {
          inInt[i] = 0;
        }
        inIndex = 0;
        return 0;
      }
    }
    if (complete) {
      digitalWrite(LED_BUILTIN, 1);
      count++;
      if (sec != millis() / 1000) {
        sec = millis() / 1000;
        fps = count + 1;
        count = 0;
      }
      Serial.print("recieved pi data\t");
      Serial.print(inIndex);
      Serial.print("\t");
      for (int i = 0; i < inIndex; i++) {
        Serial.print(inInt[i]);
        Serial.print("\t");
      }
      if (sec > 0) {
        Serial.print(count);
        Serial.print(" counts in sec \t");
      }
      Serial.println(millis());

      Serial1.println("0");     // REQUEST new data
      lastcom = millis();
      return 1;
    }
  } else {
    digitalWrite(LED_BUILTIN, 0);
    return 0;
  }
}

void delayReq(long waittime) {          // Alternative für delays die länger als 500 ms gehen und die Daten trotzdem refresht werden sollen
  long milpre = millis();
  while (millis() < milpre + waittime) {
    reqdata();
    delay(1);
  }
}
