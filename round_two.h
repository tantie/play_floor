void startGame() {
  gameActive = true;
  gameStartTime = millis();
  collectedGreens = 0;
  for (byte i = 0; i < ROWS; i++) {
    for (byte j = 0; j < COLUMNS; j++) {
      gameBoard[i][j] = 0;  // Reset game board
    }
  }
}

void spawnLava() {
  for (byte j = 0; j < COLUMNS - 1; j++) {
    for (byte i = 0; i < ROWS; i++) {
      if (safeBoard[i][j + 1] == 2) { // Если следующая ячейка зеленая
        lavaBoard[i][j] = 0; // Просто очищаем текущую ячейку лавы
      } else {
        lavaBoard[i][j] = lavaBoard[i][j + 1];
      }
    }
  }
  
  // Заполняем последний столбец лавой
  for (byte i = 0; i < ROWS; i++) {
    if (safeBoard[i][COLUMNS - 1] != 2) { // Если последняя ячейка не зеленая
      lavaBoard[i][COLUMNS - 1] = (random(100) < 10) ? 1 : 0;
    }
  }
}


void spawnSafe() {
  if (random(0, 100) < 15) {
    lastSafeRow = random(ROWS);
    lastSafeColumn = random(COLUMNS);
    safeBoard[lastSafeRow][lastSafeColumn] = 2;  // Зеленая ячейка
    lastSafeSpawnTime = millis();
  }
}


void updateLEDs() {
  for (byte i = 0; i < ROWS; i++) {
    for (byte j = 0; j < COLUMNS; j++) {
      byte cellState = lavaBoard[i][j] + safeBoard[i][j];
      switch(cellState) {
        case 0:  // Empty
          floorShow(i, j, mBlack);
          break;
        case 1:  // Lava
          floorShow(i, j, mRed);
          break;
        case 2:  // Safe
          floorShow(i, j, mGreen);
          break;
      }
    }
  }
}

void resetGame() {
  for (byte i = 0; i < ROWS; i++) {
    for (byte j = 0; j < COLUMNS; j++) {
      lavaBoard[i][j] = 0;
      safeBoard[i][j] = 0;
    }
  }
}

void clearOldSafes() {
  if (millis() - lastSafeSpawnTime >= safeSpawnInterval) {
    safeBoard[lastSafeRow][lastSafeColumn] = 0;
  }
}



void checkSafety(byte x, byte y) {
  if (safeBoard[x][y] == 2) {    
    collectedGreens++;
    MP3A.play(4);
    Serial.println(collectedGreens);
    safeBoard[x][y] = 0;
  }

  // if (lavaBoard[x][y] == 2) { //если надо вычитать
  //   collectedGreens--;
  //   lavaBoard[x][y] = 0;
  // }

}

void checkGameEnd() {
  if (collectedGreens >= TARGET_GREENS){GAME_TIME = 10000;}
  unsigned long currentTime = millis();
  if (currentTime - gameStartTime > GAME_TIME && gameActive) {
    gameActive = false;
    Serial.println(collectedGreens);
    if (collectedGreens >= TARGET_GREENS) {
      // Player won 
      endColor = mGreen;
      Serial.println("win");     
      Play(0);
      MP3A.play(3);
      MP3B.stop();
      
      if(lavaround1){
      lavaround1 = false; 
      signal2master();
      TARGET_GREENS = 20; //сколько нужно собрать зеленых
      GAME_TIME = 60000;  // 60 seconds in milliseconds
      const long lavaSpawnInterval = 120;  // скорость  для лавы
      const long safeSpawnInterval = 1300;  // скорость  для зеленых

      MP3B.stop();

      level = 21; //раунд 2 левел 1
      
      } else {
      signal2master();
      MP3B.stop();
      level = 22; //если выиграли во втором раунде
      }
      
      
    } else {
      // Player lost
      endColor = mRed;
      Serial.println("lose");
      MP3A.play(2);
      MP3B.stop();
      Play(1);
      level = 21;
    }

        for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLUMNS; j++) {
        floorShow(i, j, mBlack);
      }
    }

    currentRow = 0;
    currentColumn = 0;
    endGameEffectCounter = 5; // Начните 5 пульсаций
    
  }
}

  //стартуем лаву
void startRound2() { 
 if(gameActive){
    if (millis() - lastLavaSpawnTime >= lavaSpawnInterval) { //лава с задержкой
      spawnLava();
      lastLavaSpawnTime = millis();
    }

    if (millis() - lastSafeSpawnTime >= safeSpawnInterval) { // Создаем новую зеленую ячейку с задержкой
      safeBoard[lastSafeRow][lastSafeColumn] = 0; // Удаляем предыдущую зеленую ячейку
      spawnSafe(); 
    }

    updateLEDs(); 
    checkGameEnd(); //проверка не закончилась ли игра по времени
 }

}
 // end стартуем лаву
