int sensorNTC;
int transistoren[]{2,3,4,5,6,7,8};

void setup()
{
  pinMode(7, OUTPUT);  
  pinMode(A0, INPUT);  
  pinMode(2, OUTPUT);   
  digitalWrite(2,HIGH);    
 // for(int i : transistoren){
 //   pinMode(i, OUTPUT); 
 //   digitalWrite(i,HIGH);    
 // }
  Serial.begin(9600);     
}
void loop()
{ 
  sensorNTC=analogRead(A0);   
  Serial.print("NTC: ");
  Serial.println(sensorNTC);
  delay(100);
/*  if(sensorNTC > 350){
    digitalWrite(3,HIGH);    
  }

  if(sensorNTC > 350){
    for(int i : transistoren){
      digitalWrite(i,LOW);    
    }
  }else{
    for(int i : transistoren){
      digitalWrite(i,LOW);    
    }
  }*/
}
