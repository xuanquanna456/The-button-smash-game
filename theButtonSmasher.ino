#include<LiquidCrystal.h>
#define BUTTON_PIN_1 2
#define BUTTON_PIN_2 13
#define LED_PIN_1 3
#define LED_PIN_2 12
#define RS_PIN 6
#define E_PIN 7
#define D4 8
#define D5 9
#define D6 10
#define D7 11
bool Game_Start = false;
unsigned long LastTimeButton_1_Change = 0;
unsigned long LastTimeButton_2_Change = 0;
const unsigned long Button_Delay = 100;
const unsigned long Print_Delay = 2000;
const unsigned long GAME_TIME = 15000;
unsigned long LastTimePrint = 0;
unsigned long LastTimeToggleLed = 0;
unsigned long playerScore_1 = 0;
unsigned long playerScore_2 = 0;
unsigned long gameStartTimer = 0;
bool screen = false;
bool is_playing = false;
bool end_game = false;
LiquidCrystal lcd(RS_PIN , E_PIN , D4 , D5 , D6 , D7);
void PrintGameName(){
  lcd.clear();
  lcd.print("-BUTTON SMASHER-");
  lcd.setCursor(1 , 1);
  lcd.print("WHO IS FASTER?");
}
void setup(){
  Serial.begin(115200);
  lcd.begin(16 , 2);
  pinMode(BUTTON_PIN_1 , INPUT);
  pinMode(BUTTON_PIN_2 , INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2 , OUTPUT);
  PrintGameName();
  screen = true;
  digitalWrite(LED_PIN_1 , HIGH); digitalWrite(LED_PIN_2 , HIGH);
  delay(3000);
  digitalWrite(LED_PIN_1 , LOW); digitalWrite(LED_PIN_2 , LOW);
  delay(1000);
}
void player_play(unsigned long TIME_1 , unsigned long &playerScore_1 , unsigned long &LastTimeButton_1_Change , int BUTTON_PIN , int LED_PIN , int flag){
  if (TIME_1 - LastTimeButton_1_Change > Button_Delay and digitalRead(BUTTON_PIN) == LOW){// Update button 1 count
      playerScore_1++;
      digitalWrite(LED_PIN , HIGH);
      delay(50);
      digitalWrite(LED_PIN , LOW);
      LastTimeButton_1_Change = TIME_1;
      while(digitalRead(BUTTON_PIN) == LOW){
        delay(2);
      }
      if (flag == 1){ lcd.setCursor(3 , 0);}
      else {lcd.setCursor(13 , 0);}
      lcd.print(playerScore_1);
  }
}
void ToggleLedAndPrint(int LED_PIN , const char *s1 , const char  *s2){
    lcd.clear();
    lcd.print(s1);
    lcd.setCursor(0 , 1);
    lcd.print(s2);
    for (int i = 0 ; i < 3; i++){
      digitalWrite(LED_PIN , HIGH);
      delay(100);
      digitalWrite(LED_PIN , LOW);
    }
}
void countdown(){
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("-GAME STARTING!-");
  lcd.setCursor(0 , 1);
  for (int i = 3 ; i >= 1 ; i--){
      lcd.print(i);
      lcd.print(".. ");
      delay(1000);
  }
  lcd.print("GO!!");
  delay(500);

}
void loop(){
   if (!Game_Start){ // PRINT FIRST STAGE
      unsigned long timeNow = millis();
      if (timeNow - LastTimePrint > Print_Delay){
         if (screen == true){
           lcd.clear();
           lcd.print("PLYR 1 VS PLYR 2");
           lcd.setCursor(0 , 1);
           lcd.print("-CLICK TO START-");
         }
         else{
            PrintGameName();
         }
         //LED
         digitalWrite(LED_PIN_1 , !digitalRead(LED_PIN_1));
         digitalWrite(LED_PIN_2 , !digitalRead(LED_PIN_2));
         screen = !screen;
         LastTimePrint = timeNow;
      }
      //check BUTTON
      int BUTTON_1_STATE = digitalRead(BUTTON_PIN_1); int BUTTON_2_STATE = digitalRead(BUTTON_PIN_2);
      if (BUTTON_1_STATE == LOW or BUTTON_2_STATE == LOW){ // WAIT TO START GAME (COUNTDOWN)
        delay(200);
        countdown();
        Game_Start = true; // START GAME
      }
   }
   else{
      if (!is_playing){
        playerScore_1 = 0; // Reset play_score
        playerScore_2 = 0;
        
        lcd.clear();
        lcd.print("P1:      P2:    ");
        lcd.setCursor(0 , 1);
        lcd.print("TIME: ");
        is_playing = true;
        gameStartTimer = millis();
      }
      else{//play game
          unsigned long TIME_1 = millis();
          lcd.setCursor(6 , 1);
          unsigned long elapsed = TIME_1 - gameStartTimer;
          if(elapsed <= GAME_TIME){
              int secLeft = ((GAME_TIME - elapsed))/ 1000;
              if(secLeft < 10){
                  lcd.print(" ");
                  lcd.print(secLeft);
              }
              else{
                  lcd.print(secLeft);
              }
              // play if elapsed <= 15000
              player_play(TIME_1 , playerScore_1 , LastTimeButton_1_Change , BUTTON_PIN_1 , LED_PIN_1 , 1);// PLAYER_1 PLAY GAME
              unsigned long TIME_2 = millis(); 
              player_play(TIME_2 , playerScore_2 ,LastTimeButton_2_Change ,  BUTTON_PIN_2 , LED_PIN_2 , 2); // PLAYER_2 PLAY GAME
          }
          else{ // go to end game
              end_game = true;
              is_playing = false;
          }
      }
      // end game
      if (end_game == true){
          lcd.clear(); 
          lcd.print("GAME OVER");
          Game_Start = false;
          delay(1000);
          if (playerScore_1 > playerScore_2){
              ToggleLedAndPrint(LED_PIN_1 , "PLAYER 1 WINNER!" , "PLAYER 2 LOSES");
          }
          else if (playerScore_1 < playerScore_2){
              ToggleLedAndPrint(LED_PIN_2 , "PLAYER 1 LOSES" , "PLAYER 2 WINNER!");
          }
          else{
            lcd.clear();
            lcd.print("IT'S A TIE");
            lcd.setCursor(0 , 1);
            lcd.print("  ");
            for (int i = 0 ; i < 3; i++){
              digitalWrite(LED_PIN_1 , HIGH);
              digitalWrite(LED_PIN_2 , HIGH);
              delay(50);
              digitalWrite(LED_PIN_1 , LOW);
              digitalWrite(LED_PIN_2 , LOW);
            }
          }
          delay(2000);
          end_game = false;
      }
   }
}