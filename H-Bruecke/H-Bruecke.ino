void setup()
{
  pinMode(7, OUTPUT);  
  pinMode(8, OUTPUT);  
  pinMode(9, OUTPUT);  
}

void loop()
{  
  digitalWrite(7, LOW);  
  digitalWrite(8, HIGH);  // Motor 1 dreht jetzt in eine bestimmte Richtung.
  analogWrite(9, 255);    // Motor 1 hat maximale Leistung. 
  delay(2000);            // Fahre 2s lang.
  digitalWrite(7, LOW);    
  digitalWrite(8, LOW);   // Motor 1 ist aus. Statt diesen beiden Zeilen wÃ¤re auch "analog(9,0);" mÃ¶glich.
  delay(2000);            // Sei 2s aus.
  digitalWrite(7, HIGH);    
  digitalWrite(8, LOW);   // Motor 1 dreht jetzt in die andere Richtung.
  analogWrite(9, 127);    // Motor 1 hat halbe Leistung. 
  delay(2000);            // Fahre 2s lang langsam in die andere Richtung.
}
