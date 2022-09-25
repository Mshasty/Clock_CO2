//#define BTN_PIN 4		// кнопка (на плюс)
#define BTN1_PIN 4  // кнопка режимов
#define BTN2_PIN 2  // кнопка настройки

#include "GyverButton.h"
GButton button1(BTN1_PIN, LOW_PULL, NORM_OPEN);        // основная кнопка
GButton button2(BTN2_PIN, HIGH_PULL, NORM_OPEN);       // дополнительная кнопка

void modesTick() {
  button1.tick();
  button2.tick();

  boolean changeFlag = false;
  boolean b1Click,b2Click,b1Hold,b2Hold;

  b1Click = button1.isClick();
  b2Click = button2.isClick();
  b1Hold = button1.isHolded();
  b2Hold = button2.isHolded();

  uint8_t mn;
  
  if (b1Click) { // нажатие на кнопку 1
    Serial.println(F("Нажата кнопка 1"));
    Serial.print(F("Текущий режим: "));
    Serial.println(mode);
    switch (mode) {
      case M_MAIN:
      case M_PLOT_TH:
      case M_PLOT_TD:
      case M_PLOT_HH:
      case M_PLOT_HD:
      case M_PLOT_PH:
      case M_PLOT_PD:
      case M_PLOT_CH: 
        mode++;
        break;
      case M_PLOT_CD: 
        resetMode();
        break;
      case M_SET_SC:   // обнуление секунд
        if (mins == 59) {
          if (hrs == 23) now = DateTime(now.year(),now.month(), now.day()+1, 0, 0, 0);
          else now = DateTime(now.year(),now.month(), now.day(), now.hour()+1, 0, 0);
        } else now = DateTime(now.year(),now.month(), now.day(), now.hour(), now.minute()+1, 0); 
        secs = 0;
        break;
      case M_SET_HR:  // настройка часов
        //if (hrs == 23) { hrs = 0; now = now - TimeSpan(23*60*60); }
        if (hrs == 23) { hrs = 0; now = now - TimeSpan(0,23,0,0); }
        //else { hrs++; now = now + TimeSpan(60*60); }
        else { hrs++; now = now + TimeSpan(0,1,0,0); }
        break;
      case M_SET_MN:   // настройка минут
        if (mins == 59) { mins = 0; now = now - TimeSpan(0,0,59,0); }
        else { mins++; now = now + TimeSpan(0,0,1,0); }
        break;
      case M_SET_YR:   // настройка года
      //case 14:
        Serial.println(F("Наращиваем год"));
        //now = DateTime(now.year()+1,now.month(), min(daysInMonth(now.year()+1, now.month()),now.day()), now.hour(), now.minute(), now.second()); 
        now = DateTime(now.year()+1, now.month(), min(daysInMonth(now.year()+1, now.month()), now.day()), now.hour(), now.minute(), now.second()); 
        break;
      case M_SET_MT:   // настройка месяца
      //case 15:
        Serial.println(F("Наращиваем месяц"));
        mn = now.month();
        if (mn == 12) mn = 1;
        else mn++;
        now = DateTime(now.year(), mn, min(daysInMonth(now.year(), mn), now.day()), now.hour(), now.minute(), now.second()); 
        break;
      case M_SET_DY:   // настройка дня
        Serial.println(F("Наращиваем день"));
        int dim = daysInMonth(now.year(), now.month());
        if (now.day() >= dim) now = DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second());
        else now = DateTime(now.year(), now.month(), now.day()+1, now.hour(), now.minute(), now.second());
        break;
    }
    changeFlag = true;
    if (mode > M_SETUP) rtc.adjust(now);
  }

  if (b2Click) { // нажатие на кнопку 2
    Serial.println(F("Нажата кнопка 2"));
    Serial.print(F("Текущий режим: "));
    Serial.println(mode);
    switch (mode) {
      case M_SET_SC:   // обнуление секунд
        now = DateTime(now.year(),now.month(), now.day(), now.hour(), now.minute(), 0); 
        secs = 0;
        break;
      case M_SET_HR:  // настройка часов
        if (hrs == 0) { hrs = 23; now = now + TimeSpan(23*60*60); }
        else { hrs--; now = now - TimeSpan(60*60); }
        break;
      case M_SET_MN:   // настройка минут
        if (mins == 0) { mins = 59; now = now + TimeSpan(59*60); }
        else { mins--; now = now - TimeSpan(60); }
        break;
      case M_SET_YR:   // настройка года
        Serial.println(F("Уменьшаем год"));
        now = DateTime(now.year()-1,now.month(), min(daysInMonth(now.year()-1, now.month()), now.day()), now.hour(), now.minute(), now.second()); 
        break;
      case M_SET_MT:   // настройка месяца
        Serial.println(F("Уменьшаем месяц"));
        mn = now.month();
        if (mn == 1) mn = 12;
        else mn--;
        now = DateTime(now.year(), mn, min(daysInMonth(now.year(), mn), now.day()), now.hour(), now.minute(), now.second()); 
        break;
      case M_SET_DY:   // настройка дня
        Serial.println(F("Уменьшаем день"));
        if (now.day() == 1) now = DateTime(now.year(), now.month(), daysInMonth(now.year(), now.month()), now.hour(), now.minute(), now.second());
        else now = DateTime(now.year(), now.month(), now.day()-1, now.hour(), now.minute(), now.second());
        break;
    }
    changeFlag = true;
    rtc.adjust(now);
  }
  
  // долгое нажатие на первую кнопку - выход в главный экран
  if (b1Hold && mode > M_MAIN) {
    resetMode();
  } else if (b1Hold && mode == M_MAIN) {
    mode = M_SETUP; // режим перехода к калибровке
  } else if (b2Hold && (mode == M_SETUP)) { // долгое нажатие на вторую кнопку - калибровка
    mhz19.calibrateZero();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("MHZ-19"));
    lcd.setCursor(0, 1);
    setLED(0);
    lcd.print(F("Calibrating ZERO..."));
    delay(3000);
    resetMode();
    changeFlag = true;
  } else if (b2Hold && (mode == M_MAIN || mode > M_SETUP)) { // долгое нажатие на вторую кнопку - настройка
    if (mode == M_MAIN) {
      mode = M_SET_SC;
      TimeQuery();
    }
    else if (mode > M_SETUP) { mode++;}
    changeFlag = true;
    if (mode > M_SET_DY) resetMode();
  }

  // if (changeFlag) { screenTimer.reset(); hrs = -1; mins = -1; }
  
  if (changeFlag && mode > M_MAIN && mode < M_SETUP) {
    lcd.clear();
    loadPlot();
    redrawPlot();
  }
}

