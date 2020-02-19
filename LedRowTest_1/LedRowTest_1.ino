int ledrow1 = 30;
int ledrow2 = 31;
int ledrow3 = 32;
int ledrow4 = 33;
int ledrow5 = 34;


void setup() {
  pinMode(ledrow1,OUTPUT);
  pinMode(ledrow2,OUTPUT);
  pinMode(ledrow3,OUTPUT);
  pinMode(ledrow4,OUTPUT);
  pinMode(ledrow5,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Start");
  digitalWrite(ledrow1, HIGH);
  delay(100);
  digitalWrite(ledrow1, LOW);
  digitalWrite(ledrow2, HIGH);
  delay(100);
  digitalWrite(ledrow2, LOW);
  digitalWrite(ledrow3, HIGH);
  delay(100);
  digitalWrite(ledrow3, LOW);
  digitalWrite(ledrow4, HIGH);
  delay(100);
  digitalWrite(ledrow4, LOW);
  digitalWrite(ledrow5, HIGH);
  delay(100);
  digitalWrite(ledrow5, LOW);
}
