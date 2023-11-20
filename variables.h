#define STRIP1_TOP_PIN 23
#define STRIP2_TOP_PIN 25
#define STRIPx1_PIN 31
#define STRIPx2_PIN 27
#define STRIPx3_PIN 29
#define NUMLEDS_TOP 250
#define NUMLEDS_TILE 100
#define COLOR_DEBTH 2

#include <microLED.h>


byte level  = 5;
byte remote = 28;
byte output = 26;
byte led    = 13;

byte tile[3][8] = {
  {A0, A2, A4, A6, A12, A14, A8, A10},
  {32, 34, 36, 38, 40, 42, 44, 46},
  {47, 45, 43, 41, 39, 37, 35, 33}
};

bool tileStates[3][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

long ahora;
microLED<NUMLEDS_TOP, STRIP1_TOP_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip1_top;
microLED<NUMLEDS_TOP, STRIP2_TOP_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip2_top;

microLED<NUMLEDS_TILE, STRIPx1_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip1_tile;
microLED<NUMLEDS_TILE, STRIPx2_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip2_tile;
microLED<NUMLEDS_TILE, STRIPx3_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip3_tile;

byte video[4] = {24, 22, 20, 18};

#define COLUMNS 8
#define ROWS 3
unsigned long TARGET_GREENS = 10; //сколько нужно собрать зеленых
unsigned long GAME_TIME = 60000;  // 60 seconds in milliseconds


unsigned long startTime;
bool timerStarted = false;
bool cleanflo = true;

bool isTimerFinished(unsigned long interval) { //таймер вместо делей
  if (!timerStarted) {
    startTime = millis();
    timerStarted = true;
    //Serial.println("Таймер запущен");
    return false;
  } else if (millis() - startTime >= interval) {
    timerStarted = false; // Сбрасываем флаг, чтобы таймер мог быть перезапущен
    return true;
  } else {
    return false;
  }
}



unsigned long lastLavaSpawnTime = 0;
unsigned long lastSafeSpawnTime = 0;
const long lavaSpawnInterval = 250;  // 1  для лавы
const long safeSpawnInterval = 2000;  // 2  для зеленых
bool lavaround1 = true;

byte lavaBoard[ROWS][COLUMNS] = {0};
byte safeBoard[ROWS][COLUMNS] = {0};

byte lastSafeRow = 0;
byte lastSafeColumn = 0;



byte collectedGreens = 0;
unsigned long gameStartTime;
//bool gameActive = true; //если фалс то игра будет стартовать при каком-то условии
bool gameActive = true;
bool previousGameActive = false;

byte gameBoard[ROWS][COLUMNS];

bool sensorStates[3][8] = {
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false}
};







byte currentRow = 0;
byte currentColumn = 0;
int endGameEffectCounter = 0;
int endColor;




void floorShow(byte x, byte y, uint32_t color) {
  switch (y) {
    case 0:
      for (int w = 0; w < 11; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 1:
      for (int w = 11; w < 22; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 2:
      for (int w = 22; w < 33; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 3:
      for (int w = 33; w < 44; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 4:
      for (int w = 44; w < 56; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 5:
      for (int w = 56; w < 67; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 6:
      for (int w = 67; w < 78; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    case 7:
      for (int w = 78; w < 89; w++) {
        if (x == 0) strip1_tile.leds[w] = color;
        if (x == 1) strip2_tile.leds[w] = color;
        if (x == 2) strip3_tile.leds[w] = color;
      }
      break;
    default:
      for (int w = 0; w < 89; w++) {
        if (x == 0) strip1_tile.leds[w] = mBlack;
        if (x == 1) strip2_tile.leds[w] = mBlack;
        if (x == 2) strip3_tile.leds[w] = mBlack;
      }
      break;
  }
}








// Глобальные переменные для управления игрой
enum GameState {
  INIT_GAME,
  BLINK_SEQUENCE,
  DISPLAY_SEQUENCE,
  WAIT_PLAYER_ACTION,
  CHECK_SOLUTION
};
GameState currentState = INIT_GAME;

//int gameTime = 30; //время игры
//int duration = 10; //время показывания правильного варианта


// цвета: mWhite, mSilver, mGray, mBlack, mRed, mMaroon, mOrange, mYellow, mOlive, mLime, mGreen, mAqua, mTeal, mBlue, mNavy, mMagenta, mPurple


//mRed, mBlue, mPurple, mYellow, mGreen
// Правильная последовательность цветов
int correctSequence[3][8] = {
  {mRed, mBlue, mSilver, mBlue, mRed, mGreen, mSilver, mGreen},
  {mSilver, mBlack, mBlack, mBlack, mBlack, mBlack, mBlack, mSilver},
  {mGreen, mSilver, mRed, mBlue, mSilver, mGreen, mRed, mBlue}
};

int whightSequence[3][8] = {
  {mSilver, mSilver, mSilver, mSilver, mSilver, mSilver, mSilver, mSilver},
  {mSilver, mBlack, mBlack, mBlack, mBlack, mBlack, mBlack, mSilver},
  {mSilver, mSilver, mSilver, mSilver, mSilver, mSilver, mSilver, mSilver}
};




//новая матрица для работы с цветами
void setColor(byte x, byte y, int color) {
  int startLED = 0;
  int endLED = 0;

  switch (y) {
    case 0:
      startLED = 0;
      endLED = 11;
      break;
    case 1:
      startLED = 11;
      endLED = 22;
      break;
    case 2:
      startLED = 22;
      endLED = 33;
      break;
    case 3:
      startLED = 33;
      endLED = 44;
      break;
    case 4:
      startLED = 44;
      endLED = 55;
      break;
    case 5:
      startLED = 55;
      endLED = 66;
      break;
    case 6:
      startLED = 66;
      endLED = 78;
      break;
    case 7:
      startLED = 78;
      endLED = 89;
      break;
  }

  for (int w = startLED; w < endLED; w++) {
    if (x == 0) strip1_tile.leds[w] = color;
    if (x == 1) strip2_tile.leds[w] = color;
    if (x == 2) strip3_tile.leds[w] = color;
  }
}

void FloorClear() {
  strip1_tile.clear();
  strip2_tile.clear();
  strip3_tile.clear();

  strip1_tile.show();
  strip2_tile.show();
  strip3_tile.show();

}

uint32_t getColor(byte x, byte y) {
  // Serial.println("getColor");

  int ledIndex; // индекс первого светодиода в ячейке

  switch (y) {
    case 0:
      ledIndex = 0;
      break;
    case 1:
      ledIndex = 11;
      break;
    case 2:
      ledIndex = 22;
      break;
    case 3:
      ledIndex = 33;
      break;
    case 4:
      ledIndex = 44;
      break;
    case 5:
      ledIndex = 55;
      break;
    case 6:
      ledIndex = 66;
      break;
    case 7:
      ledIndex = 78;
      break;
  }

  if (x == 0) return strip1_tile.leds[ledIndex];
  if (x == 1) return strip2_tile.leds[ledIndex];
  if (x == 2) return strip3_tile.leds[ledIndex];

  return 0;  // Значение по умолчанию (не должно достигаться)
}
