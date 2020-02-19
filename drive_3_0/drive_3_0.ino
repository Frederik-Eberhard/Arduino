#define sender 2
#define empfaenger 3

const int m_a1 = 42;    //motor A signal 1
const int m_a2 = 43;    //motor A signal 2
const int m_a  = A15;    //motor A speed
const int m_b1 = 44;    //motor B signal 1
const int m_b2 = 45;    //motor B signal 2
const int m_b  = A14;    //motor B speed

const int m_cam  = 45;  //camera motor
int messung, abstand;



void setup()
{
  pinMode(m_a1, OUTPUT);  
  pinMode(m_a2, OUTPUT);  
  pinMode(m_a, OUTPUT);
  pinMode(m_b1, OUTPUT);  
  pinMode(m_b2, OUTPUT);  
  pinMode(m_b, OUTPUT);  
  
  pinMode(m_cam, OUTPUT);
  Serial.begin(9600); 
  pinMode(sender, OUTPUT);                    // Digital 2 ist ein Sender (PWM).
  pinMode(empfaenger, INPUT);                 // Digiatl 3 ist ein Empfänger. 
  delay(10);
}

void loop()
{ 
  Serial.println("1");
  drive(1,255);
  delay(2000);
  Serial.println("2");
  drive(2,255);
  delay(2000);
  Serial.println("3");
  drive(3,255);
  delay(2000);
  Serial.println("4");
  drive(4,255);
  delay(2000);
  cam(0);
}

void cam(int dir){
  if(dir == 0){
    digitalWrite(m_cam, 0);
  }
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
    digitalWrite(m_a1, HIGH);  
    digitalWrite(m_a2, LOW);  // Motor 1 dreht jetzt vorwärts.
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
    digitalWrite(m_a1, LOW);  
    digitalWrite(m_a2, HIGH);  // Motor 1 dreht jetzt rückwärts.
    analogWrite(m_a, s);       // Motor 1 Leistung. 
    digitalWrite(m_b1, LOW);  
    digitalWrite(m_b2, HIGH);  // Motor 2 dreht jetzt vorwärts.
    analogWrite(m_b, s);       // Motor 2 Leistung. 
  }
}

/*
  digitalWrite(sender, HIGH);                 // Ein Ultraschallsignal wird gesendet.
  delayMicroseconds(100);                     // Er macht es 100ms=0,1s.
  digitalWrite(sender, LOW);                  // Das Ultraschallsignal wird beendet.
  messung = pulseIn(empfaenger, HIGH);        // Laufzeitmessung
  abstand = messung/57,7;                     // Umrechnung in cm
  Serial.print(abstand);                      // Ausgabe auf den Monitor
  Serial.println(" cm"); 
  delay(20);
*/
