int leds[][3][3] = {{{0,0,0},{0,0,0},{0,0,0}},    
                    {{0,0,0},{0,0,0},{0,0,0}},
                    {{0,0,0},{0,0,0},{0,0,0}}};         //255 -> aus, 0-> an
int board[][3]  = {{0,0,0},
                   {0,0,0},
                   {0,0,0}};  //0-> nobody, 1-> player1, 2-> player2
boolean pl  = true;           //player: true -> player1, false -> player2
int win = 0;
int winanfrm = 0;             //winAnimationFrame

void setup() {                                                                                                 //mögliche Modi: Tick Tack Toe -> PvP,PvC,CvC, Simon Says
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

void loop() {
  int s = get_board();
  if(s == 1){   //go back
    reset();
  }
  
  if(win == 0){
    win = get_win();
  }else{
    winAnimation(win);  
  }
  Serial.print("Signal:");
  Serial.println(s);
  
  Serial.print("Winner:");
  Serial.println(win);
  boardToLeds();
  show();
}


void reset(){
  for(int i = 0; i < 9; i++){
      leds[i % 3][i / 3][0] = 0;             //255 -> aus, 0-> an
      leds[i % 3][i / 3][1] = 0;
      leds[i % 3][i / 3][2] = 0;
      board[i % 3][i / 3]   = 0;             //0-> nobody, 1-> player1, 2-> player2
  }
  
  pl  = true;                 //player: true -> player1, false -> player2
  win = 0;
  winanfrm = 0;                //winAnimationFrame
}

int get_board(){
  for(int i = 0; i < 9; i++){
    if(digitalRead(5+4*i)==HIGH && board[i % 3][i / 3] == 0){  //if it hasn't been pressed befor
      if(pl)
        board[i % 3][i / 3] = 1;    //CAREFULL: fliped along x-Axis
      else
        board[i % 3][i / 3] = 2;    //CAREFULL: fliped along x-Axis
      pl = !pl;
    }
  }  
  Serial.print(board[0][0]);
  Serial.print(board[1][0]);
  Serial.println(board[2][0]);
  Serial.print(board[0][1]);
  Serial.print(board[1][1]);
  Serial.println(board[2][1]);
  Serial.print(board[0][2]);
  Serial.print(board[1][2]);
  Serial.println(board[2][2]);
  if(digitalRead(5+4*2) && digitalRead(5+4*5) && digitalRead(5+4*8)){
    return 1;
  }
  return 0;
}

int get_win(){
  if((board[0][0] == 1 && board[0][1] == 1 && board[0][2] == 1)
   ||(board[1][0] == 1 && board[1][1] == 1 && board[1][2] == 1)
   ||(board[2][0] == 1 && board[2][1] == 1 && board[2][2] == 1)
   ||(board[0][0] == 1 && board[1][0] == 1 && board[2][0] == 1)
   ||(board[0][1] == 1 && board[1][1] == 1 && board[2][1] == 1)
   ||(board[0][2] == 1 && board[1][2] == 1 && board[2][2] == 1)
   ||(board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1)
   ||(board[0][2] == 1 && board[1][1] == 1 && board[2][0] == 1))
      return 1;
  if((board[0][0] == 2 && board[0][1] == 2 && board[0][2] == 2)
   ||(board[1][0] == 2 && board[1][1] == 2 && board[1][2] == 2)
   ||(board[2][0] == 2 && board[2][1] == 2 && board[2][2] == 2)
   ||(board[0][0] == 2 && board[1][0] == 2 && board[2][0] == 2)
   ||(board[0][1] == 2 && board[1][1] == 2 && board[2][1] == 2)
   ||(board[0][2] == 2 && board[1][2] == 2 && board[2][2] == 2)
   ||(board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2)
   ||(board[0][2] == 2 && board[1][1] == 2 && board[2][0] == 2))
      return 2;
  return 0;
}


void winAnimation(int w){
   for(int i = 0; i < 9; i++){
      board[i % 3][i / 3] = w; 
   }
}

void boardToLeds(){
  for(int i = 0; i < 9; i++){
    if(board[i % 3][i / 3] == 1){
      leds[i % 3][i / 3][0] = 0;
      leds[i % 3][i / 3][1] = 255;
      leds[i % 3][i / 3][2] = 255;  
    }else if(board[i % 3][i / 3] == 2){
      leds[i % 3][i / 3][0] = 255;
      leds[i % 3][i / 3][1] = 255;
      leds[i % 3][i / 3][2] = 0;  
    }
  }
}


void show(){
  for(int i = 0; i < 9; i++){   
    digitalWrite(2+4*i,leds[i % 3][i / 3][0]);
    digitalWrite(3+4*i,leds[i % 3][i / 3][1]);  //grün
    digitalWrite(4+4*i,leds[i % 3][i / 3][2]); 
    Serial.print(leds[i % 3][i / 3][0]);Serial.print(" ");Serial.print(leds[i % 3][i / 3][1]);Serial.print(" ");Serial.print(leds[i % 3][i / 3][2]);
    Serial.println(i);
  }  
}
