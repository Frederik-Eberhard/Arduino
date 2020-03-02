#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <MPU6050_tockn.h>
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
#include <Wire.h>

#define teensyAdr 0x09  //Teensy Address
#define gyroAdr 0xc3    //Gyro Address
#define displayAdr 0x3c //Display Address
int zahl = 0;
int Variable1 = 0;       //Temp Var für Display zum hochzählen
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

//--create display and gyro
Adafruit_SSD1306 display(128, 64);  // Create display
MPU6050 gyro(Wire);
 

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

  //--Display
  display.begin(SSD1306_SWITCHCAPVCC, displayAdr);  // Initialize display with the I2C address of 0xc4
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                               // To override this behavior (so text will run off the right side of the display - useful for
                               // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                               // with setTextWrap(true).
  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)

  //--Gyro
  Wire.begin(gyroAdr);       // for MPU6050
  gyro.begin();
  gyro.calcGyroOffsets(true);

  Serial.begin(9600);
  
  bootup_animation();
  Serial.println("Bereit");
}

void loop() {
  ledLeiste(255);
  getGyro();
  drive(getData());
  showDisplay();  
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
  int i2,i3;
  b1 = 0;                           
  i2 = 255;
  i3 = 255;
  
  if(a > 90)                  //links
    i2 = constrain((255 - (pow(a-90,2) * 51 / 490)),-255,255);
  else
    i3 = constrain((255 - (pow(a-90,2) * 51 / 490)),-255,255);
  
  Serial.print("i2 :");
  Serial.print(i2);
  Serial.print("i3 :");
  Serial.println(i3);
  b1 = (i2 > 0)? (i3 > 0)? 3:2 : (i3 > 0)? 1:0;     //11  , 10 , 01 , 0 --> Motor Turning (0 = back ; 1 = forward)
  b2 = (i2 < 0)? i2*-1:i2;
  b3 = (i3 < 0)? i3*-1:i3;
  long l = (b1 << 8) | b2;
  l =  l << 8 | b3;
  return l;
}

void turn(byte dir, byte sb, byte sa){
  int a1,a2,b1,b2;
  switch(dir){                                                 // B   |  A
    case 0: b1 = HIGH; b2 = LOW; a1 = LOW; a2 = HIGH; break;   //back | back
    case 1: b1 = HIGH; b2 = LOW; a1 = HIGH; a2 = LOW; break;   //back | forw
    case 2: b1 = LOW; b2 = HIGH; a1 = LOW; a2 = HIGH; break;   //forw | back
    case 3: b1 = LOW; b2 = HIGH; a1 = HIGH; a2 = LOW; break;   //forw | forw
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
  Wire.requestFrom(teensyAdr,1);
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

//--Gyro-Sensor----------------------
void getGyro(){
  gyro.update();
  Serial.print("angleX : ");
  Serial.print(gyro.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(gyro.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(gyro.getAngleZ());  
}

void  showDisplay(){
  Variable1++;  // Increase variable by 1
  if(Variable1 > 150)  // If Variable1 is greater than 150
  {
    Variable1 = 0;  // Set Variable1 to 0
  }
  // Convert Variable1 into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  dtostrf(Variable1, 3, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)
  display.clearDisplay();  // Clear the display so we can refresh
  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
  // Print text:
  display.setCursor(0, 10);  // (x,y)
  display.println("Hello");  // Text or value to print
  // Draw triangle:
  display.drawTriangle(40,40,   50,20,   60,40, WHITE);  // Draw triangle. X, Y coordinates for three corner points defining the triangle, followed by a color
  // Draw filled triangle:
  display.fillTriangle(0,63,   15,45,   30,63, WHITE);  // Draw filled triangle. X, Y coordinates for three corner points defining the triangle, followed by a color
  // Draw line:
  display.drawLine(40, 63, 70, 63, WHITE);  // Draw line (x0,y0,x1,y1,color)
  // Draw circle:
  display.drawCircle(47, 36, 20, WHITE);  //  Draw circle (x,y,radius,color). X and Y are the coordinates for the center point
  // Draw a filled circle:
  display.fillCircle(12, 27, 10, WHITE);  // Draw filled circle (x,y,radius,color). X and Y are the coordinates for the center point
 // Draw rounded rectangle and fill:
  display.fillRoundRect(58, 0, 18, 18, 5, WHITE);  // Draw filled rounded rectangle (x,y,width,height,color)
                                                   // It draws from the location to down-right
  // Draw rectangle:
  display.drawRect(79, 0, 49, 27, WHITE);  // Draw rectangle (x,y,width,height,color)
                                           // It draws from the location to down-right
  display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  // Print variable with left alignment:
  display.setCursor(83, 20);  // (x,y)
  display.println(Variable1);  // Text or value to print
  // Draw rounded rectangle:
  display.drawRoundRect(79, 37, 49, 27, 8, WHITE);  // Draw rounded rectangle (x,y,width,height,radius,color)
                                                    // It draws from the location to down-right
  // Print variable with right alignment:
  display.setCursor(83, 57);  // (x,y)
  display.println(string);  // Text or value to print
  display.display();  // Print everything we set previously
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
  if(a < 105 && a > 75)     //15 | -15
    return 1;
  if(a < 125 && a > 55)     //35 | -35
    return (a > 90)? 2:3;
  if(a < 150 && a > 30)     //60 | -60
    return (a > 90)? 4:5;  
  if(a < 180 && a > -1)     //90 | -90
    return (a > 90)? 6:7;
  return 10;   
}

//---------------OLD STUFF---------------------















/*
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
*/
