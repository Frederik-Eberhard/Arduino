#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <MPU6050_tockn.h>
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
#include <Wire.h>
#include <Servo.h>

#define teensyAdr 0x09  //Teensy Address
#define gyroAdr 0xc3    //Gyro Address
#define displayAdr 0x3c //Display Address


//--functions
byte getData(int num = 0); 
void drive(int a, bool f = false, bool r = false);

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
const int serva = 38;
const int servb = 39;
const int servs = 5;   //unnessesary
const int ledp = 40;
const int ledn = 41;
const int bright = 4;   //brightness

//--define ultraschall
const int sender = 46;
const int empfaenger = 53;

//--create display, gyro, servo
Adafruit_SSD1306 display(128, 64);  // Create display
MPU6050 gyro(Wire);
Servo camServo;

//--Global Variables
const float SPEEDTHROTTLE_DEFAULT = 2;    //Teiler, der 255 Fullspeed 
const int RAMPE_ANGLE = 25;     //Grad, bei der Rampe
const int SERVO_DEFAULT = 85;     //Range (110-70)

float SpeedThrottle = SPEEDTHROTTLE_DEFAULT;
//int originalgyrodata[3];
int gyrodata[3];

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
  pinMode(serva,OUTPUT);
  pinMode(servb,OUTPUT);
  pinMode(servs, OUTPUT);

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
  /*for(int i = 0; i < 3; i++){
    originalgyrodata[i] = 0;  
  }*/
  Wire.begin(gyroAdr);       // for MPU6050
  gyro.begin();
  gyro.calcGyroOffsets(true);
  getGyro();
  /*for(int i = 0; i < 3; i++){
    originalgyrodata[i] = gyrodata[i];  
  }*/
  //--Servo
  camServo.attach(9);
  setServ();
  //--Ultraschall
  pinMode(sender, OUTPUT);
  pinMode(empfaenger, INPUT);

  Serial.begin(115200);
  
  bootup_animation();
  Serial.println("Bereit");
}

void loop() {
  ledLeiste(255);
  getGyro();
  int dist = getUltraschall();
  if(dist < 12){
    flasche();
  }
  //rampe();
  drive(getData(), false, false);
  Serial.println();  
  delay(10);
}

void drive(int a, bool f, bool r){
  ledRowControl(a);
  if(a < 181){       //181 --> 180° Kurve  | 182 --> Back | 183 --> keine Linie
    long dir = getDir(a);
    Serial.print("dir: ");
    Serial.print(dir,BIN);
    byte s2 = dir & 255;
    dir = dir >> 8;
    byte s1 = dir & 255;
    dir = dir >> 8;
    byte b = dir;
    turn(b,s1,s2);
    Serial.print(" b: ");
    Serial.print(b);
    Serial.print(", s1: ");
    Serial.print(s1);
    Serial.print(", s2: ");
    Serial.print(s2);
    showDisplay(a, s1, s2, b, f, r);    //f -> Flasche    r -> Rampe
  }else{
    if(a == 181){
      kurve180();
    }else if(a == 182){     //Rampe
      turn(0,255/SpeedThrottle,255/SpeedThrottle);
    }else if(a == 183){     //Keine Linie
      keineLinie();
    }
  }
}

long getDir(int a){ 
  byte b1,b2,b3;              //motorOn, SpeedB, SpeedA
  int i2,i3;
  b1 = 0;                           
  i2 = 255/SpeedThrottle;
  i3 = 255/SpeedThrottle;
  
  if(a > 90)                  //links
    i2 = constrain((255 - (pow(a-90,2) * 51 / 290)),-255,255)/SpeedThrottle;
  else
    i3 = constrain((255 - (pow(a-90,2) * 51 / 290)),-255,255)/SpeedThrottle;
  
  Serial.print(" i2 :");
  Serial.print(i2);
  Serial.print(" i3 :");
  Serial.print(i3);
  b1 = (i2 > 0)? (i3 > 0)? 3:2 : (i3 > 0)? 1:0;     //11  , 10 , 01 , 0 --> Motor Turning (0 = back ; 1 = forward)
  b2 = (i2 < 0)? i2*-1:i2;
  b3 = (i3 < 0)? i3*-1:i3;
  long l = (b1 << 8) | b2;
  l =  l << 8 | b3;
  return l;
}

void turn(byte dir, byte sa, byte sb){
  int a1,a2,b1,b2;
  switch(dir){                                                 // B   |  A
    case 0: b1 = HIGH; b2 = LOW; a1 = LOW; a2 = HIGH; break;   //back | back
    case 1: b1 = HIGH; b2 = LOW; a1 = HIGH; a2 = LOW; break;   //back | forw
    case 2: b1 = LOW; b2 = HIGH; a1 = LOW; a2 = HIGH; break;   //forw | back
    case 3: b1 = LOW; b2 = HIGH; a1 = HIGH; a2 = LOW; break;   //forw | forw
    case 4: b1 = LOW; b2 = LOW; a2 = LOW; a2 = LOW;   break;   //stop | stop
  }
  
  digitalWrite(m_a1, a1);  
  digitalWrite(m_a2, a2);         // Motor A dreht.
  analogWrite(m_a, sa);           // Motor A SA% Leistung. 
  digitalWrite(m_b1, b1);      
  digitalWrite(m_b2, b2);         // Motor B dreht.
  analogWrite(m_b, sb);           // Motor B SB% Leistung.     
}

void kurve180(){
  Serial.print("180° Kurve");
  drive(90,false, false);
  delay(400);  
  int angleStart = gyrodata[2];
  while(abs(gyrodata[2] - angleStart) < 180){
    Serial.println("180° Turning");
    getGyro();
    drive(0,false,false);
    delay(10);  
  } 
}

void keineLinie(){
  Serial.print("KEINE LINIE");
  turn(4,0,0);
  setServ(SERVO_DEFAULT+20);//turn up to see line
  delay(100);
  setServ();
  while(getData() == 183){    //dreh, bis du wieder linie siehst / wenn du die Linie nach der Lücke gesehen hast und dann wieder runter guckst, musst du auch der bis du wieder Linie siehst weiter fahren
    Serial.print("KEINE LINIE");
    drive(getData(1));
  }
}

//--I2C------------------------------
byte getData(int num = 0){ 
  if(num)
    sendData(num, true);
  Wire.requestFrom(teensyAdr,1);
  byte data;
  while(Wire.available()){
    data = Wire.read();
    Serial.print(" Got: ");
    Serial.print(data);  
  }
  delay(10);
  return data;
}

void sendData(int num, bool request){
  Wire.beginTransmission(teensyAdr); // transmit to device #8
  if(request)
    Wire.write(1);              //1 --> Teensy get me following: num
  Wire.write(num);              // sends one byte
  Wire.endTransmission();    // stop transmitting
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
  gyrodata[0] = gyro.getAngleX();
  gyrodata[1] = gyro.getAngleY();
  gyrodata[2] = gyro.getAngleZ();
  
  Serial.print(" angleX : ");
  Serial.print(gyrodata[0]);
  Serial.print("\tangleY : ");
  Serial.print(gyrodata[1]);
  Serial.print("\tangleZ : ");
  Serial.print(gyrodata[2]);  
}

//--Ultraschall----------------------
int getUltraschall(){
  int messung,a;
  digitalWrite(sender, HIGH);
  delayMicroseconds(100);
  digitalWrite(sender, LOW);
  messung = pulseIn(empfaenger,HIGH);
  a = messung/58,2;
  if(a < 0)
    a = 100;
  delay(20);
  Serial.print("  ");
  Serial.print(a);
  Serial.print(" cm  ");

  return a;
}

//--Servo
void setServ(){
  setServ(SERVO_DEFAULT);  
}
void setServ(int angle){
  digitalWrite(serva, HIGH);      
  digitalWrite(servb, LOW);         // Motor B dreht.
  analogWrite(servs, 255);           // Motor B SB% Leistung.     

  camServo.write(angle);
  delay(15);
}
//--Rampe----------------------------
void rampe(){
  if(gyrodata[1] < -RAMPE_ANGLE){   //Gyro Y auf der Rampe runter
//    setServ(SERVO_DEFAULT+20);
//    SpeedThrottle *= 2;
  }else if(gyrodata[1] > RAMPE_ANGLE){
    SpeedThrottle /= 2;
    Serial.println("RAMPE_HOCH");    
  }else{
    setServ();          //Reset Servo
    SpeedThrottle = SPEEDTHROTTLE_DEFAULT;
  }
}

//--Flasche--------------------------
void flasche(){
  drive(179);
  delay(1000);
  int leftUlt = getUltraschall();
  drive(1);
  delay(2000);
  int rightUlt = getUltraschall();
  drive(179);
  delay(1000);
  if(leftUlt > 12 && rightUlt > 12){
    drive(182, true, false);  //back
    delay(250);
    drive(179, true, false);
    delay(2200);
    drive(90, true, false);
    delay(1000);
    drive(1, true, false);
    delay(2000);
    drive(90, true, false);
    delay(1000);
    drive(1, true, false);
    delay(1000);
    drive(90, true, false);
    delay(800);
    drive(1, true, false);            //Hier sollte die Linie wiedergefunden werden   
  }
}

//--Display--------------------------
void  showDisplay(int data, int s1, int s2, int dir, bool flasche, bool rampe){
  switch(dir){
    case 0: s1 *= -1; s2 *= -1; break;  
    case 1: s1 *= -1; break;  
    case 2: s2 *= -1; break;  
  }

  display.clearDisplay();  // Clear the display so we can refresh


  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0

  
  display.drawCircle(15,15,25,WHITE);   //Data
  display.setCursor(0,20);
  display.println(data);
  
  display.setCursor(55,10);             //Gyroscope
  display.println("x");
  display.setCursor(80,10);
  display.println("y");
  display.setCursor(105,10);
  display.println("z");
  display.setCursor(50,30);
  display.println(gyrodata[0]);
  display.setCursor(75,30);
  display.println(gyrodata[1]);
  display.setCursor(100,30);
  display.println(gyrodata[2]);
 
  display.setCursor(10,60);             //Speeds
  display.println(s1);
  display.setCursor(80,60);
  display.println(s2);

  if(flasche)
    display.fillTriangle(50,60, 60,50,  70,60, WHITE);    //Flasche
    
  display.drawRect(50,0,75,40,WHITE);
  display.display();
}
//--LED Row Controll Void------------
void bootup_animation(){
  for(int i = 0; i < 11; i++){
    ledRowControl(i,false);
    delay(100);  
  }  
  for (int pos = 70; pos <= 110; pos += 1) { // goes from 0 degrees to 180 degrees
    setServ(pos);
  }
  for (int pos = 110; pos >= 70; pos -= 1) { // goes from 0 degrees to 180 degrees
    setServ(pos);
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
