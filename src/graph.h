// переменные для вывода
float dispTemp;
byte dispHum;
int dispPres;
int dispCO2;
int dispRain;
boolean dotFlag;

// массивы графиков
int tempHour[15], tempDay[15];
int humHour[15], humDay[15];
int pressHour[15], pressDay[15];
int co2Hour[15], co2Day[15];
int delta;
uint32_t pressure_array[6];
uint32_t sumX, sumY, sumX2, sumXY;
float a, b;
byte time_array[6];

// символы
// график
byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};

// цифры
uint8_t LT[8] = {0b00111,  0b01111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
uint8_t UB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};
uint8_t RT[8] = {0b11100,  0b11110,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
uint8_t LL[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b01111,  0b00111};
uint8_t LB[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
uint8_t LR[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11110,  0b11100};
uint8_t UMB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
uint8_t LMB[8] = {0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};

void drawDig(byte dig, byte x, byte y, boolean clean) {

  lcd.setCursor(x + 3, y); // set cursor to column 0, line 0 (first row)
  if (clean) lcd.write(32);
  lcd.setCursor(x + 3, y + 1); // set cursor to colum 0, line 1 (second row)
  if (clean) lcd.write(32);
  switch (dig) {
    case 0:
      lcd.setCursor(x, y); // set cursor to column 0, line 0 (first row)
      lcd.write(0);  // call each segment to create
      lcd.write(1);  // top half of the number
      lcd.write(2);
      lcd.setCursor(x, y + 1); // set cursor to colum 0, line 1 (second row)
      lcd.write(3);  // call each segment to create
      lcd.write(4);  // bottom half of the number
      lcd.write(5);
      break;
    case 1:
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.write(0);
      lcd.write(32);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(255);
      lcd.write(32);
      break;
    case 2:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(255);
      lcd.write(7);
      lcd.write(7);
      break;
    case 3:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 4:
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(255);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(255);
      break;
    case 5:
      lcd.setCursor(x, y);
      lcd.write(255);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 6:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 7:
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(1);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(0);
      lcd.write(32);
      break;
    case 8:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 9:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 10:
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      break;
  }
}

#if (WEEK_LANG == 0)
static const char *dayNames[]  = {
  " Sun ",
  " Mon ",
  " Tue ",
  " Wed ",
  " Thi ",
  " Fra ",
  " Sat ",
};
#else
static const char *dayNames[]  = {
  " SUN ",
  " MON ",
  " TUE ",
  " WED ",
  " THI ",
  " FRA ",
  " SAT ",
};
#endif

void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int *plot_array, String label) {
  int max_value = -32000;
  int min_value = 32000;

  for (byte i = 0; i < 15; i++) {
    if (plot_array[i] > max_value) max_value = plot_array[i];
    if (plot_array[i] < min_value) min_value = plot_array[i];
  }
  lcd.setCursor(16, 0); lcd.print(max_value);
  lcd.setCursor(16, 1); lcd.print(label);
  lcd.setCursor(16, 2); lcd.print(plot_array[14]);
  lcd.setCursor(16, 3); lcd.print(min_value);

  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    int fill_val = plot_array[i];
    fill_val = constrain(fill_val, min_val, max_val);
    byte infill, fract;
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    if (plot_array[i] > min_val)
      infill = floor((float)(plot_array[i] - min_value) / (max_value - min_value) * height * 10);
    else infill = 0;
    fract = (float)(infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;

    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor(i, (row - n));        // заполняем полными ячейками
        lcd.write(0);
      }
      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor(i, (row - n));
        if (fract > 0) lcd.write(fract);          // заполняем дробные ячейки
        else lcd.write(16);                       // если дробные == 0, заливаем пустой
        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor(i, (row - k));
          lcd.write(16);
        }
        break;
      }
    }
  }
}

void loadClock() {
  lcd.createChar(0, LT);
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, UMB);
  lcd.createChar(7, LMB);
}

void loadPlot() {
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}

void redrawPlot() {
  lcd.clear();
  switch (mode) {
    case M_PLOT_TH: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tempHour, "t hr");
      break;
    case M_PLOT_TD: drawPlot(0, 3, 15, 4, TEMP_MIN, TEMP_MAX, (int*)tempDay, "t day");
      break;
    case M_PLOT_HH: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humHour, "h hr");
      break;
    case M_PLOT_HD: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, (int*)humDay, "h day");
      break;
    case M_PLOT_PH: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)pressHour, "p hr");
      break;
    case M_PLOT_PD: drawPlot(0, 3, 15, 4, PRESS_MIN, PRESS_MAX, (int*)pressDay, "p day");
      break;
    case M_PLOT_CH: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Hour, "c hr");
      break;
    case M_PLOT_CD: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, (int*)co2Day, "c day");
      break;
  }
}

void drawClock( byte hours, byte minutes, byte x, byte y) {

  if (mode == M_MAIN || mode == M_SET_SC || mode > M_SET_MN) { 
    if (hrs != hours) {
      if (hours / 10 == 0) drawDig(10, x, y, true);
      else drawDig(hours / 10, x, y, true);
      drawDig(hours % 10, x + 4, y, true);
      hrs = hours;
    }
    if (mins != minutes) {
      drawDig(minutes / 10, x + 8, y, true);
      drawDig(minutes % 10, x + 12, y, false);
      mins = minutes;
    }
  } else {
    if (mode == M_SET_HR) {
      hrs = hours;
      mins = minutes;
      if (dotFlag) {
        if (hours / 10 == 0) drawDig(10, x, y, true);
        else drawDig(hours / 10, x, y, true);
        drawDig(hours % 10, x + 4, y, true);
      } else {
        drawDig(10, x, y, true);
        drawDig(10, x + 4, y, true);
      }
    }

    if (mode == M_SET_MN) {
      if (hrs != hours) {
        if (hours / 10 == 0) drawDig(10, x, y, true);
        else drawDig(hours / 10, x, y, true);
        drawDig(hours % 10, x + 4, y, true);
        hrs = hours;
      }        
      if (dotFlag) {
        drawDig(minutes / 10, x + 8, y, true);
        drawDig(minutes % 10, x + 12, y, false);
      } else {
        drawDig(10, x + 8, y, true);
        drawDig(10, x + 12, y, true);
      }
      mins = minutes;
    }

  }
}

void drawDots(byte x, byte y) {
  byte code;
  if (dotFlag) code = 32;
  else code = 165;
  lcd.setCursor(x, y);
  lcd.write(code);
  lcd.setCursor(x, y + 1);
  lcd.write(code);
}

void drawDate() { // рисуем дату, месяц и год
  
  if (mode == M_MAIN || mode == M_SET_SC) {
    lcd.setCursor(15, 0);
    if (now.day() < 10) lcd.print(0);
    lcd.print(now.day());
    lcd.print(".");
    if (now.month() < 10) lcd.print(0);
    lcd.print(now.month());
  
    if (DISP_MODE == 0) {
      lcd.setCursor(16, 1);
      lcd.print(now.year());
    } else if (DISP_MODE == 1) {
      lcd.setCursor(15, 1);
      int dayofweek = now.dayOfTheWeek();
      lcd.print(dayNames[dayofweek]);
    }
  } else { // Для режимов настройки времени

    byte d = 1;
    byte m = 1;
    byte y = 1;

    if (!dotFlag) // мигаем изс=меняемым параметром
    switch (mode) {
      case M_SET_DY:
        d = 0;
        break;
      case M_SET_MT:
        m = 0;
        break;
      case M_SET_YR:
        y = 0;
        break;
    }

    lcd.setCursor(15, 0);
    if (d)
    {
      if (now.day() < 10) lcd.print(0);
      lcd.print(now.day());
    } else lcd.print("  ");
    lcd.print(".");
    
    if (m)
    {
      if (now.month() < 10) lcd.print(0);
      lcd.print(now.month());
    } else lcd.print("  ");
  
    lcd.setCursor(16, 1);
    if (y) lcd.print(now.year());
    else lcd.print("    ");
  }
}

void drawSensors() {
#if (DISPLAY_TYPE == 1)
  // дисплей 2004
  lcd.setCursor(0, 2);
  lcd.print(String(dispTemp, 1));
  lcd.write(223);
  lcd.print("C");
  lcd.setCursor(7, 2);
  lcd.print(" " + String(dispHum) + "% ");

#if (CO2_SENSOR == 1)
  if (dispCO2 < 1000) lcd.print(" ");
  lcd.print(String(dispCO2) + " ppm");
#endif

  lcd.setCursor(0, 3);
  lcd.print(String(dispPres) + " mm  rain ");
  lcd.print(F("       "));
  lcd.setCursor(13, 3);
  lcd.print(String(dispRain) + "%");

#else
  // дисплей 1602
  lcd.setCursor(0, 0);
  lcd.print(String(dispTemp, 1));
  lcd.write(223);
  lcd.setCursor(6, 0);
  lcd.print(String(dispHum) + "% ");

#if (CO2_SENSOR == 1)
  lcd.print(String(dispCO2) + "ppm");
  if (dispCO2 < 1000) lcd.print(" ");
#endif

  lcd.setCursor(0, 1);
  lcd.print(String(dispPres) + " mm  rain ");
  lcd.print(String(dispRain) + "% ");
#endif
}

void plotSensorsTick() {
  // 4 минутный таймер
  if (hourPlotTimer.isReady()) {
    for (byte i = 0; i < 14; i++) {
      tempHour[i] = tempHour[i + 1];
      humHour[i] = humHour[i + 1];
      pressHour[i] = pressHour[i + 1];
      co2Hour[i] = co2Hour[i + 1];
    }
    tempHour[14] = dispTemp;
    humHour[14] = dispHum;
    co2Hour[14] = dispCO2;

    if (PRESSURE) pressHour[14] = dispRain;
    else pressHour[14] = dispPres;
  }

  // 1.5 часовой таймер
  if (dayPlotTimer.isReady()) {
    long averTemp = 0, averHum = 0, averPress = 0, averCO2 = 0;

    for (byte i = 0; i < 15; i++) {
      averTemp += tempHour[i];
      averHum += humHour[i];
      averPress += pressHour[i];
      averCO2 += co2Hour[i];
    }
    averTemp /= 15;
    averHum /= 15;
    averPress /= 15;
    averCO2 /= 15;

    for (byte i = 0; i < 14; i++) {
      tempDay[i] = tempDay[i + 1];
      humDay[i] = humDay[i + 1];
      pressDay[i] = pressDay[i + 1];
      co2Day[i] = co2Day[i + 1];
    }
    tempDay[14] = averTemp;
    humDay[14] = averHum;
    pressDay[14] = averPress;
    co2Day[14] = averCO2;
  }

  // 10 минутный таймер
  if (predictTimer.isReady()) {
    // тут делаем линейную аппроксимацию для предсказания погоды
    long averPress = 0;
    long Pa;
    for (byte i = 0; i < 10; i++) {
      //bme.takeForcedMeasurement();
      Pa = bmp.readPressure(); // переводим в мм.рт.ст.
      averPress += Pa;
      delay(1);
    }
    averPress /= 10;

    for (byte i = 0; i < 5; i++) {                   // счётчик от 0 до 5 (да, до 5. Так как 4 меньше 5)
      pressure_array[i] = pressure_array[i + 1];     // сдвинуть массив давлений КРОМЕ ПОСЛЕДНЕЙ ЯЧЕЙКИ на шаг назад
    }
    pressure_array[5] = averPress;                    // последний элемент массива теперь - новое давление
    sumX = 0;
    sumY = 0;
    sumX2 = 0;
    sumXY = 0;
    for (int i = 0; i < 6; i++) {                    // для всех элементов массива
      sumX += time_array[i];
      sumY += (long)pressure_array[i];
      sumX2 += time_array[i] * time_array[i];
      sumXY += (long)time_array[i] * pressure_array[i];
    }
    a = 0;
    a = (long)6 * sumXY;             // расчёт коэффициента наклона приямой
    a = a - (long)sumX * sumY;
    a = (float)a / (6 * sumX2 - sumX * sumX);
    delta = a * 6;      // расчёт изменения давления
    dispRain = map(delta, -250, 250, 100, 0);  // пересчитать в проценты
    //Serial.println(String(pressure_array[5]) + " " + String(delta) + " " + String(dispRain));   // дебаг
  }
}

