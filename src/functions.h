bool isLeapYear(int yr) {
  return (yr % 4) == 0 && (yr % 100) != 0 || (yr % 400) == 0;
}

int daysInMonth(int yr, int mn) {
byte domN[12] = {31,28,31,30,31,30,31,31,30,31,30,31};  
byte domL[12] = {31,29,31,30,31,30,31,31,30,31,30,31};  
  if (isLeapYear(yr)) return domL[mn];
  else return domN[mn];
}

void TimeQuery() {
  now = rtc.now();
  secs = now.second();
  mins = now.minute();
  hrs = now.hour();
}

void clockTick() {
  dotFlag = !dotFlag;
  if (dotFlag) {          // каждую секунду пересчёт времени
    secs++;
    if (secs > 59) {      // каждую минуту
      now = rtc.now();
      secs = now.second(); 
    }
  }
  
  if (mode == M_MAIN || mode == M_SET_SC) {
    drawClock(now.hour(), now.minute(), 0, 0);
    drawDots(7, 0);
    drawDate();
    if (DISP_MODE == 2) {
      lcd.setCursor(17, 1);
      if (mode == M_SET_SC && dotFlag)
        lcd.print(F("  "));
      else {  
        if (secs < 10) lcd.print("0");
        lcd.print(secs);
      }
    }
  } else if (mode >= M_SET_HR && mode <= M_SET_DY) {
    drawClock(now.hour(), now.minute(), 0, 0);
    drawDots(7, 0);
    drawDate();
  }  
  
  if (dispCO2 < 800) setLED(2);       // зелёный
  else if (dispCO2 < 1200) setLED(4); // синий (жёлтый)
  else if (dispCO2 >= 1200) {
    setLED(1);                        // красный
    if (dispCO2 >= 1600) {            // мигаем красным светодиодом
      if (dotFlag) setLED(1);
      else setLED(0);
    }
  }
}

void resetMode() {
  mode = M_MAIN;
  hrs = -1;
  mins = -1;
  //screenTimer.reset();
  lcd.clear();
  loadClock();
  clockTick();
  drawDate();
  drawSensors();
}

void readSensors() {
  //bme.takeForcedMeasurement();
  dispTemp = dht.readTemperature();
  dispHum = dht.readHumidity();
  float Pa = (bmp.readPressure()/133.33);
  //Serial.println(String(Pa, 1) + " t=" + String(bmp.readTemperature(), 1));   // дебаг
  dispPres = Pa;
#if (CO2_SENSOR == 1)
  dispCO2 = mhz19.getPPM();

  if (dispCO2 < 800) setLED(2);       // Зелёный до 800
  else if (dispCO2 < 1200) setLED(4); // Синий до 1200
  else setLED(1);                     // Красный после 1200
#endif
}