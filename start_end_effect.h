void pulseEffect1() { //эффект по завершению игры
  static unsigned long lastPulseTime = 0;
  unsigned long currentMillis = millis();
  
  // Если прошло 500 мс с последней пульсации, выполните пульсацию
  if (currentMillis - lastPulseTime > 500) {
    lastPulseTime = currentMillis;
    for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLUMNS; j++) {
        floorShow(i, j, endColor);
      }
    }
    strip1_tile.show();
    strip2_tile.show();
    strip3_tile.show();
    //delay(100); // Держите цвет 100 мс
    for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLUMNS; j++) {
        floorShow(i, j, mBlack);
      }
    }
    strip1_tile.show();
    strip2_tile.show();
    strip3_tile.show();
    endGameEffectCounter--;
  }
}



void pulseEffect() { 

  static unsigned long lastWaveTime = 0;
  unsigned long currentMillis = millis();
  
  // Если прошло 50 мс с последней волны, создайте новую волну
  if (currentMillis - lastWaveTime > 50) {
    lastWaveTime = currentMillis;

    // Сброс, если вся доска была освещена
    if (currentRow >= ROWS) {
      currentRow = 0;
      currentColumn = 0;
      endGameEffectCounter--;
      if (endGameEffectCounter <= 0) {
        for (byte i = 0; i < ROWS; i++) {
          for (byte j = 0; j < COLUMNS; j++) {
            floorShow(i, j, endColor);
          }
        }
        strip1_tile.show();
        strip2_tile.show();
        strip3_tile.show();
        return;
      }
    }
    
    floorShow(currentRow, currentColumn, endColor);
    
    currentColumn++;
    if (currentColumn >= COLUMNS) {
      currentColumn = 0;
      currentRow++;
    }

    strip1_tile.show();
    strip2_tile.show();
    strip3_tile.show();
  }
}


// Волна начинается с центра
byte centerX = COLUMNS / 2;
byte centerY = ROWS / 2;
byte currentRadius = 0;

void pulseEffectFromCenter() {

  static unsigned long lastWaveTime = 0;
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastWaveTime > 250) {
    lastWaveTime = currentMillis;
    
    for (byte i = 0; i < ROWS; i++) {
      for (byte j = 0; j < COLUMNS; j++) {
        // Проверка, находится ли ячейка на текущем радиусе
        byte distance = max(abs(i - centerY), abs(j - centerX));
        if (distance == currentRadius) {
          floorShow(i, j, endColor);
        }
      }
    }

    strip1_tile.show();
    strip2_tile.show();
    strip3_tile.show();

    currentRadius++;

    if (currentRadius > max(centerX, centerY)) {
      currentRadius = 0;
      endGameEffectCounter--;
      if (endGameEffectCounter <= 0) {
        // Завершите эффект, подсветив все ячейки
        for (byte i = 0; i < ROWS; i++) {
          for (byte j = 0; j < COLUMNS; j++) {
            floorShow(i, j, endColor);
          }
        }
        strip1_tile.show();
        strip2_tile.show();
        strip3_tile.show();
        return;
      }
    }
  }
}
