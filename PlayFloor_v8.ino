#define message "PlayFloor_v8"

#include <DFPlayerMini_Fast.h>

DFPlayerMini_Fast MP3A;
DFPlayerMini_Fast MP3B;



#include "variables.h" //переменные


void Play(byte wich) {
 // Serial.println("play_video");
  digitalWrite(video[wich], HIGH);
  delay(100);
  digitalWrite(video[wich], LOW);
}

#include "round_one.h" //раунд 1
#include "round_two.h" //раунд 2
#include "round_three.h" //раунд 3
#include "start_end_effect.h" //эффекты пол
#include "runningdots.h" //облака

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.println(message);

  delay(300);
  MP3A.begin(Serial2,  false);
  delay(200);

  delay(300);
  MP3B.begin(Serial3,  false);
  delay(200);

  delay(100);
  MP3A.volume(16);
  delay(100);

  delay(100);
  MP3B.volume(18);
  delay(100);

  //MP3A.stop();
  //MP3A.play(1);
 // delay(5000);



  pinMode(remote, INPUT_PULLUP);
  pinMode(output, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  digitalWrite(output, LOW);

  for (byte v = 0; v < 4; v++) {
    pinMode(video[v], OUTPUT);
    digitalWrite(video[v], LOW);
  }



  for (byte x = 0; x < 3; x++) {
    for (byte y = 0; y < 8; y++) {
      pinMode(tile[x][y], INPUT_PULLUP);
      Serial.println("tile = " + String(x) + "," + String(y) + " = " + String(digitalRead(tile[x][y]) ? " HIGH" : " LOW"));
    }
    Serial.println("--------");
    delay(50);
  }
  strip1_top.setBrightness(160);
  strip2_top.setBrightness(160);
  strip1_tile.setBrightness(200);
  strip2_tile.setBrightness(200);
  strip3_tile.setBrightness(200);
  //delay(3000); //

  strip1_top.clear();
  strip2_top.clear();
  MP3B.stop();
  MP3A.stop();
  //delay(5000);
  FloorClear();  // Очищаем пол
 //  delay(100);
 // startTime = millis(); // запоминаем время старта для таймеров
//  strip1_top.fill(0,100, mGreen);
//   strip2_top.fill(0,100, mRed);
  strip1_top.show();
  strip2_top.show();
  fillFloor(mRed);
 //delay(4000000);
}

bool level15play = true;

void loop() {
 

  if (level == 5) { //престарт сервисный режим
   //fillFloor(mRed); //возможно проблема в этом
  
  }

  if (level == 10) {
    if (!previousGameActive) { //престарт уровень 1
      checkSensorsAndControlLED();

    } else {
      //Serial.println("game rd1 ");
      MP3A.play(1);
      signal2master();     
      level = 15;  
    }

  }

  
  if (level == 15) {
   
    fillFloor(mGreen);
    
    if (level15play) {
    Play(0);
    level15play = false;  
    }
  
  if (isTimerFinished(29000)) { // Устанавливаем таймер на 8 секунд
    // Код, который выполняется после n секунд
      
      startGame(); //очищаем игру перед стартом
      MP3B.play(1);
        strip1_top.clear();
        strip2_top.clear();
        strip1_top.show();
        strip2_top.show();
      level = 20;
  } else {runningDots_int();
  } // Код, который выполняется во время таймера

  }
//   if (level == 19) { //тестовый
// runningDots_int1();
//   }

  if (level == 20) {
//    bool rpestertround2_1 = true;
  // if(rpestertround2_1) startGame(); rpestertround2_1 = false;
    //Serial.println("level 20 round2 1 time");
    startRound2(); //раунд 2
    //runningDots_int();
    
  for (byte x = 0; x < 3; x++) {
    for (byte y = 0; y < 8; y++) {
      if (digitalRead(tile[x][y]) ) {
        checkSafety(x, y); //считает зеленые 
      }
    }
    strip1_tile.show();
    strip2_tile.show();
    strip3_tile.show();
  }  
  }

  if (level == 21) { // если раунд 2 левел 1
  
  if (isTimerFinished(8000)) { // Устанавливаем таймер на 8 секунд
    startGame();
    MP3B.play(1);
      strip1_top.clear();
      strip2_top.clear();
        strip1_top.show();
        strip2_top.show();
    level = 20;
  } else {

          if (!gameActive && endGameEffectCounter > 0) { 
        pulseEffectFromCenter();
        runningDots_int1(); //моргаем цветом
            strip1_tile.show();
            strip2_tile.show();
            strip3_tile.show();

      }

  }

  }


    if (level == 22) { // если раунд 2 левел 2
  
  if (isTimerFinished(8000)) { // Устанавливаем таймер на 8 секунд
    startGame();
    MP3B.play(2);
      strip1_top.clear();
      strip2_top.clear();
              strip1_top.show();
        strip2_top.show();
    level = 30;
  } else {

          if (!gameActive && endGameEffectCounter > 0) { 
        pulseEffectFromCenter();
        runningDots_int1(); //моргаем цветом
            strip1_tile.show();
            strip2_tile.show();
            strip3_tile.show();
      }

  }

  }


  if (level == 30) {
    startRound3();  // раунд 3 цвета
  }

  if (level == 31) { //если не выиграл 3й раунд   
    if (isTimerFinished(8000)) { // Устанавливаем таймер на 8 секунд
  if (cleanflo) { // очищаемм пол
  FloorClear();
  MP3B.play(2);
  strip1_top.clear();
  strip2_top.clear();
          strip1_top.show();
        strip2_top.show();
  cleanflo = false;
  }
  } else { runningDots_int1(); //моргаем цветом
  }
  if (!cleanflo){
 
    startRound3();
    }
  }


  if (level == 40) {
   
   Serial.println("level 40");
    fillFloor(mGreen);
    runningDots_int();
    


}

catchRemote();

}// end loop



void  catchRemote() {
  if ((Serial.available() > 0 && Serial.read() == '1')) { //просто сериал порт
        level = 10;
      }
  
  
  if (digitalRead(remote) == LOW) {
    while (digitalRead(remote) == LOW) {
      delay(50);
    }

  // if ((Serial.available() > 0 && Serial.read() == '2')) {
  //   Play(0);
  // }

    // if (digitalRead(remote) == LOW || (Serial.available() > 0 && Serial.read() == '1')) {
    // if (digitalRead(remote) == LOW) {
    //   // Ожидание отпускания кнопки пульта
    //   while (digitalRead(remote) == LOW) {
    //     delay(50);
    //   }
    //   Serial.flush();
    //   Serial.println("remote");
    // } else {
    //   Serial.println("Received 1 from serial");
    // }




    if (level == 31) { 
      MP3A.stop();
      MP3B.stop();
      Play(0);
      
      Serial.println("Move by operator !");
      digitalWrite(led,      HIGH);
      level = 40;
    }

    if (level == 30) { 
      MP3A.stop();
      MP3B.stop();
      Play(0);
      
      Serial.println("Move by operator !");
      digitalWrite(led,      HIGH);
      level = 40;
    }
       
    if (level == 20) { 
      MP3A.stop();
      MP3B.stop();
      //MP3B.play(2);
      Play(2);
      delay(100);
      
      Serial.println("Move by operator !");
      digitalWrite(led,      HIGH);
      level = 22;
    }

    if (level == 10) { 
      
      Serial.println("Move by operator !");
      digitalWrite(led,      HIGH);
      level = 15;
    }

    if (level == 5) { 
      
      Serial.println("Move by operator !");
      digitalWrite(led,      HIGH);
      level = 10;
    }
  }
}


void signal2master(){
  digitalWrite(output, HIGH);
  delay(100);
  digitalWrite(output, LOW);
}
