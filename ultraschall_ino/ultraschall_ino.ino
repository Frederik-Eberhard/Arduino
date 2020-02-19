
#define sender 2
#define empfaenger 3

int messung, abstand;

void setup()
{                                 
  Serial.begin(9600);                         // Kommunikationskanal 9600
  pinMode(sender, OUTPUT);                    // Digital 2 ist ein Sender (PWM).
  pinMode(empfaenger, INPUT);                 // Digiatl 3 ist ein Empfänger.
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

}         

 


