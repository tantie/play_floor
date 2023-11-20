void runningDots() {
  static byte counter = 0;
  // перемотка буфера со сдвигом (иллюзия движения пикселей)
  for (int i = 0; i < NUMLEDS_TOP - 1; i++) {
    strip1_top.leds[i] = strip1_top.leds[i + 1];
    strip2_top.leds[i] = strip2_top.leds[i + 1];
  }

  // каждый третий вызов - последний пиксель красным, иначе чёрным
  if (counter % 3 == 0) {
    strip1_top.leds[NUMLEDS_TOP - 1] = mRed;
    strip2_top.leds[NUMLEDS_TOP - 1] = mBlue;

  }  else {
    strip1_top.leds[NUMLEDS_TOP - 1] = mBlack;
    strip2_top.leds[NUMLEDS_TOP - 1] = mBlack;
  }
  counter++;
//  delay(10);   // дополнительная задержка
}

void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 50;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip1_top.setBrightness(bright);
  strip2_top.setBrightness(bright);
}

void breathing2() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 20;    // 5 - множитель скорости изменения

  if (bright > 150) {
    bright = 150;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip1_top.setBrightness(bright);
  strip2_top.setBrightness(bright);
}

void colorCycle() {
  static byte counter = 0;
  strip1_top.fill(mWheel8(counter));
  strip2_top.fill(mWheel8(counter));
  counter += 4;
}

void colormmm() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS_TOP; i++) {
  strip1_top.fill(endColor);
  strip2_top.fill(endColor);
  }
  counter += 3;
}

void rainbow() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS_TOP; i++) {
    strip1_top.set(i, mWheel(counter + i * 255 / NUMLEDS_TOP));   // counter смещает цвет
    strip2_top.set(i, mWheel(counter + i * 255 / NUMLEDS_TOP));   // counter смещает цвет
  }
  counter += 3;   // counter имеет тип byte и при достижении 255 сбросится в 0
}




void redgrom() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS_TOP; i++) {
    strip1_top.set(i, mWheel(3, counter + i * 100 / NUMLEDS_TOP));   // counter смещает цвет
    strip2_top.set(i, mWheel(3, counter + i * 100 / NUMLEDS_TOP));   // counter смещает цвет
    //mWheel(int color, uint8_t bright=255); // colors 0-1530 + brightness
  }
  counter += 6;
}

void greengrom() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS_TOP; i++) {
    
    strip1_top.set(i, counter + i * 100 / NUMLEDS_TOP, mRed);   // counter смещает цвет
   // strip2_top.set(i, mWheel(30, counter + i * 100 / NUMLEDS_TOP));   // counter смещает цвет
    //mWheel(int color, uint8_t bright=255); // colors 0-1530 + brightness
    
  }
  counter += 6;
}

void runningDots_int() {
    breathing();
    colorCycle();

    strip1_top.show(); 
    strip2_top.show();   
}

void runningDots_int1() {
    colormmm();
    breathing2();
    strip1_top.show(); 
    strip2_top.show();   

}
void runningDots_int2() {
  endColor = mRed;
    colormmm();
    breathing2();
    strip1_top.show(); 
    strip2_top.show();   

}
