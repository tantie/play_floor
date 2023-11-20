void checkSensorsAndControlLED() { //раунд 1 заполнение всех цветов
  for (byte row = 0; row < 3; row++) {
    for (byte column = 0; column < 8; column++) {
      if (digitalRead(tile[row][column])) {
        floorShow(row, column, mLime);
        sensorStates[row][column] = true;
      }
    }
  }
  strip1_tile.show();
  strip2_tile.show();
  strip3_tile.show();


  gameActive = true; //
//  Serial.println("game true");


  for (byte row = 0; row < 3; row++) {
    for (byte column = 0; column < 8; column++) {
      if (!sensorStates[row][column]) {
          gameActive = false;
//        Serial.println("game false");
        break;
      }
    }
    if (!gameActive) {
      break;
    }
  }

  if (gameActive) {
    digitalWrite(led, HIGH);

  } else {
    digitalWrite(led, LOW);
  }
  previousGameActive = gameActive;
  //Serial.println("this point");

}

