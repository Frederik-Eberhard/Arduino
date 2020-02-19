int leds[][3]={{0,0,0},  
               {0,0,0},
               {0,0,0}};   //0 =  aus, 1 = rot, 2 = gruen, 3 = blau
boolean pl1[][3]={{false,false,false},
                  {false,false,false},
                  {false,false,false}};
boolean pl2[][3]={{false,false,false},  
                  {false,false,false},
                  {false,false,false}};
boolean currplay= true; //current player -> player one
int animationframe = 0;

//boolean get_input(boolean);

void setup(){                                                                                                 //mögliche Modi: Tick Tack Toe -> PvP,PvC,CvC, Simon Says
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
  if(get_input(currplay)){
    currplay = !currplay;   //other players turn
  }
  int winner = false;//check_win(); //0 = noch keiner, 1 = player1, 2 = player2    
  //Serial.println(winner);
  get_leds();  
  Serial.println(leds[0][0]);
  Serial.println(leds[1][0]);
  Serial.println(leds[2][0]);
  Serial.println(leds[0][1]);
  Serial.println(leds[1][1]);
  Serial.println(leds[2][1]);
  Serial.println(leds[0][2]);
  Serial.println(leds[1][2]);
  Serial.println(leds[2][2]);
  show(winner);
}

boolean get_input(boolean player1){  //player1 -> which player
  for(int i = 0; i < 9; i++){  
    if(digitalRead(5+4*i) == HIGH){  //if toucher is pressed
      if(player1)
        pl1[i % 3][(i / 3)] = true;
      else
        pl2[i % 3][(i / 3)] = true;
      return true;
    }
  }
  return false;
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

void get_leds(){
  for(int i = 0; i < 9; i++){  
    if(pl1[i % 3][(i / 3)]){        //if player one -> red
      leds[i % 3][(i / 3)] = 1;
    }else if(pl2[i % 3][(i / 3)]){  //else if player two -> green
      leds[i % 3][(i / 3)] = 2;   
    }else{                          //else  -> out
      leds[i % 3][(i / 3)] = 0; 
    }
  }
}

void show(int winner){  
  //Serial.println("SHOW");
  if(winner == 0){   //no winner yet
    for(int i = 0; i < 9; i++){  
    // Serial.println("leds");
    // Serial.println(leds[i % 3][(i / 3)]);
     if(leds[i % 3][(i / 3)] == 0){
       digitalWrite(2+4*i,LOW);  //weiß
       digitalWrite(3+4*i,LOW);
       digitalWrite(4+4*i,LOW);
       Serial.println("AUS");        
     }else if(leds[i % 3][(i / 3)] == 1){
       digitalWrite(2+4*i,LOW);  //rot
       digitalWrite(3+4*i,HIGH);
       digitalWrite(4+4*i,HIGH);
       Serial.println("ROT");  
       //Serial.println(2+4*i);
     }else if(leds[i % 3][(i / 3)] == 2){
       digitalWrite(2+4*i,HIGH);
       digitalWrite(3+4*i,LOW);  //grün
       digitalWrite(4+4*i,HIGH);
       Serial.println("GRUEN");  
       //Serial.println(3+4*i);
     }else if(leds[i % 3][(i / 3)] == 3){
       digitalWrite(2+4*i,HIGH);
       digitalWrite(3+4*i,HIGH);  
       digitalWrite(4+4*i,LOW);  //blau
       //Serial.println(3+4*i);
     }
    } 
  }else if(winner == 1 || winner == 2){
    winanimation();  
  }
}

void winanimation(){
  
  animationframe++;
}
