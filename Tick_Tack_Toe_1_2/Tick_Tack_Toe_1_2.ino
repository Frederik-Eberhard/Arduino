int leds[][3]={{0,0,0},  
               {0,0,0},
               {0,0,0}};   //0 =  aus, 1 = rot, 2 = gruen, 3 = blau
boolean pl1[][3]={{false,false,false},
                  {false,false,false},
                  {false,false,false}};
boolean pl2[][3]={{false,false,false},  
                  {false,false,false},
                  {false,false,false}};

void setup(){
  Serial.begin(9600);          
  for(int i = 1; i <= 36; i++){    //36 Ports (2R,3G,4B,5S)
    if(i%4 == 0){
      pinMode(i+1,INPUT);
      Serial.println("IN");
    }
    else{
      pinMode(i+1,OUTPUT);
      Serial.println("OUT");
    }
  }
}

void loop(){
  get_leds();  
  int winner = check_win(); //0 = noch keiner, 1 = player1, 2 = player2    
  show();
}

void get_leds(){  
  for(int i = 0; i < 9; i++){  
    if(digitalRead(5+4*i) == HIGH){  //if toucher is pressed
      leds[i % 3][(i / 3)] = 1;
    }else{
      leds[i % 3][(i / 3)] = 0;  
    }
  }
}

int check_win(){
  if((pl1[0][0] && pl1[0][1] && pl1[0][2])
   ||(pl1[1][0] && pl1[1][1] && pl1[1][2])
   ||(pl1[2][0] && pl1[2][1] && pl1[2][2])
   ||(pl1[0][0] && pl1[1][0] && pl1[2][0])
   ||(pl1[0][1] && pl1[1][1] && pl1[2][1])
   ||(pl1[0][2] && pl1[1][2] && pl1[2][2])
   ||(pl1[0][0] && pl1[1][1] && pl1[2][2])
   ||(pl1[0][2] && pl1[1][1] && pl1[0][2])){
    return 1;  
  }
  if((pl2[0][0] && pl2[0][1] && pl2[0][2])
   ||(pl2[1][0] && pl2[1][1] && pl2[1][2])
   ||(pl2[2][0] && pl2[2][1] && pl2[2][2])
   ||(pl2[0][0] && pl2[1][0] && pl2[2][0])
   ||(pl2[0][1] && pl2[1][1] && pl2[2][1])
   ||(pl2[0][2] && pl2[1][2] && pl2[2][2])
   ||(pl2[0][0] && pl2[1][1] && pl2[2][2])
   ||(pl2[0][2] && pl2[1][1] && pl2[0][2])){
    return 2; 
  }
  return 0;
}

void show(){  
  for(int i = 0; i < 9; i++){  
    if(leds[i % 3][(i / 3)] = 1){
      digitalWrite(2+4*i,LOW);  //rot
      digitalWrite(3+4*i,HIGH);
      digitalWrite(4+4*i,HIGH);
      Serial.println("ROT");  
      //Serial.println(2+4*i);
    }else if(leds[i % 3][(i / 3)] = 2){
      digitalWrite(2+4*i,HIGH);
      digitalWrite(3+4*i,LOW);  //grün
      digitalWrite(4+4*i,HIGH);
      Serial.println("GRUEN");  
      //Serial.println(3+4*i);
    }else if(leds[i % 3][(i / 3)] = 3){
      digitalWrite(2+4*i,HIGH);
      digitalWrite(3+4*i,HIGH);  
      digitalWrite(4+4*i,LOW);  //blau
      //Serial.println(3+4*i);
    }*/
  }

}
