void displaySequence() {//правильный вариант на 10 секунд
//Serial.println("displaySequence");
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            // Устанавливаем цвет на основе правильной последовательности
            setColor(row, col, correctSequence[row][col]);
        }
    }
                strip1_tile.show();
                strip2_tile.show();
                strip3_tile.show();

}

void displayWhightSeq() {//белые квадраты
//Serial.println("displaySequence");
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            // Устанавливаем цвет на основе правильной последовательности
            setColor(row, col, whightSequence[row][col]);
        }
    }
                strip1_tile.show();
                strip2_tile.show();
                strip3_tile.show();

}

bool checkSolution() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            uint32_t currentColor = getColor(row, col);
            // Пропускаем черные и белые ячейки
            // if (currentColor == mBlack || correctSequence[row][col] == mSilver) {
            //     continue;
            // }

            if (currentColor != correctSequence[row][col]) {
                if (correctSequence[row][col] == -2048 && currentColor == 63488) {
                    // Специальная проверка для случая, когда expected -2048, got 63488
                    continue;
                    return false;
                }

                 if (correctSequence[row][col] == -14824 && currentColor == 50712) {
                    // Специальная проверка для случая, когда expected -2048, got 63488
                    continue;
                    return false;
                }


                // Serial.print("Mismatch at (");
                // Serial.print(row);
                // Serial.print(", ");
                // Serial.print(col);
                // Serial.print("): expected ");
                // Serial.print(correctSequence[row][col]);
                // Serial.print(", got ");
                // Serial.println(currentColor);
                return false;  // Возвращает false, если любой цвет не соответствует правильной последовательности
            }
        }
    }
    return true;  // Возвращает true, если все цвета соответствуют правильной последовательности
}


void fillFloor(int color) { //окончание игры красный либо зеленый
//Serial.println("end game");
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            setColor(row, col, color);
        }
                strip1_tile.show();
                strip2_tile.show();
                strip3_tile.show();
    }
   // delay (5000);
}





#define NUM_COLORS 4
uint32_t activeColors[NUM_COLORS] = {mRed, mBlue, mSilver, mGreen};
byte currentColorIndex[3][8] = {0};


unsigned long lastChangeTime[3][8] = {0};

void changeTileColor(byte x, byte y) {
    // Если цвет ячейки mBlack, вернитесь без изменения цвета
    if (getColor(x, y) == mBlack) {
        return;
    }

    // Проверьте, прошло ли достаточно времени с момента последнего изменения цвета для этой ячейки
    if (millis() - lastChangeTime[x][y] > 500) {
        currentColorIndex[x][y] = (currentColorIndex[x][y] + 1) % NUM_COLORS;
        setColor(x, y, activeColors[currentColorIndex[x][y]]);
    
        // Обновите светодиодные полосы после изменения цвета
        strip1_tile.show();
        strip2_tile.show();
        strip3_tile.show();

        // Обновите время последнего изменения для этой ячейки
        lastChangeTime[x][y] = millis();
    }
}

void startRound3() {
    unsigned long currentMillis = millis();
    
    switch (currentState) {
        case INIT_GAME:
            Serial.println("startRound3");
            currentState = BLINK_SEQUENCE;
            break;
            
        case BLINK_SEQUENCE: //моргаем три раза
            if (currentMillis - gameStartTime < 2000) { 
              displaySequence();
            }else if(currentMillis - gameStartTime < 3000){ 
              FloorClear();
            }else if(currentMillis - gameStartTime < 5000){
              displaySequence();
            }else if(currentMillis - gameStartTime < 6000){ 
              FloorClear();
            } else if(currentMillis - gameStartTime < 8000){
              displaySequence();
            } else if(currentMillis - gameStartTime < 9000){
                FloorClear();
            } else {
                currentState = DISPLAY_SEQUENCE;
                gameStartTime = currentMillis;
            }
            break;
            
        case DISPLAY_SEQUENCE:
            if (currentMillis - gameStartTime < 20000) { // Display for 10 seconds
                displaySequence();
            } else if(currentMillis - gameStartTime < 21000){
                FloorClear(); 
            } else {
                //randomizeColors();
                displayWhightSeq();
                currentState = WAIT_PLAYER_ACTION;
                gameStartTime = currentMillis;
            }
            break;

        case WAIT_PLAYER_ACTION:

            if (checkSolution()) {
        // Если решение правильное, сразу переходим к проверке решения
        currentState = CHECK_SOLUTION;
        break;
              }

            if (currentMillis - gameStartTime > 45000) {//время игры
                currentState = CHECK_SOLUTION;
            } else {
                // Обработка нажатия ячеек
                for (byte x = 0; x < 3; x++) {
                    for (byte y = 0; y < 8; y++) {
                        if (digitalRead(tile[x][y])) {
                            changeTileColor(x, y);
                        }
                    }
                }
            }
            break;

        case CHECK_SOLUTION:
            // Проверка решения игрока
            if (checkSolution()) {
                // Если верно, включаем все ячейки зеленым цветом
                fillFloor(mGreen);
                Play(0);
                MP3A.play(3);
                signal2master();
                MP3B.stop();
                level = 40;
            } else {
                // Если неверно или время истекло, включаем все ячейки красным цветом
                fillFloor(mRed);
                MP3A.play(2);
                Play(1);
                cleanflo = true;
                MP3B.stop();
                level = 31;
            }
            // Сброс состояния игры для следующего раунда
            currentState = INIT_GAME;
            break;
    }

}


// void randomizeColors() { //рандомные цвета
// Serial.println("randomizeColors");
//     int colors[] = {mRed, mBlue, mSilver, mGreen};
//     int numColors = sizeof(colors) / sizeof(colors[0]);

//     for (int row = 0; row < 3; row++) {
//         for (int col = 0; col < 8; col++) {
//             if (correctSequence[row][col] != mBlack) {
//                 int randomColor = colors[random(numColors)];  // Выбираем случайный цвет из массива
//                 setColor(row, col, randomColor);

//             }
//         }
//     }
//                 strip1_tile.show();
//                 strip2_tile.show();
//                 strip3_tile.show();
// }

// void sandClockEffect(int gameTime) { // таймер времени недоделан

//   Serial.println("sandClockEffect");
//     int totalTime = gameTime * 1000;  // Общее время игры в миллисекундах
//     int updateTime = totalTime / 8;  // Интервал времени для обновления каждой пары пикселей

//     for (int i = 0; i < 4; i++) {  // Цикл для 4 пар пикселей
//         delay(updateTime);
//         for (int row = 0; row < 3; row++) {
//             if (correctSequence[row][i] == mBlack) {
//                 setColor(row, i, mBlack);
//             }
//             if (correctSequence[row][7 - i] == mBlack) {
//                 setColor(row, 7 - i, mBlack);
//             }
//         }
        
//         // Обновляем светодиодные полосы после изменения цвета
//         strip1_tile.show();
//         strip2_tile.show();
//         strip3_tile.show();
//     }
// }
