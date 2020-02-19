#define sender 2
#define empfaenger 3

int messung, abstand;
int m1= 30;
int m2= 31;
int m3= 10;

void setup()
{                                 
  Serial.begin(9600);                         // Kommunikationskanal 9600
  pinMode(sender, OUTPUT);                    // Digital 2 ist ein Sender (PWM).
  pinMode(empfaenger, INPUT);                 // Digital 3 ist ein Empfänger 
  pinMode(m1, OUTPUT); 
  pinMode(m2, OUTPUT);  
  pinMode(m3, OUTPUT);                         // Digiatl 3 ist ein Empfänger.
}
 
void loop()
{
  digitalWrite(sender, HIGH);                 // Ein Ultraschallsignal wird gesendet.
  delayMicroseconds(100);                     // Er macht es 100ms=0,1s.
  digitalWrite(sender, LOW);                  // Das Ultraschallsignal wird beendet.
  messung = pulseIn(empfaenger, HIGH);        // Laufzeitmessung
  abstand = messung/57,7;                     // Umrechnung in cm
  Serial.print(abstand);                      // Ausgabe auf den Monitor
  Serial.println(" cm"); 
  delay(20);

  if(abstand< 7)
  {
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    analogWrite(m3, 255);
    delay(4000);
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    analogWrite(m3, 255);
    delay(4000);
  }
 
}

   



  
  

 


