/*
  Скетч к проекту "Домашняя метеостанция"
  Страница проекта (схемы, описания): https://alexgyver.ru/meteoclock/
  Исходники на GitHub: https://github.com/AlexGyver/MeteoClock
  Нравится, как написан и закомментирован код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  http://AlexGyver.ru/
*/

/*
  Время и дата устанавливаются атвоматически при загрузке прошивки (такие как на компьютере)
  График всех величин за час и за сутки (усреднённые за каждый час)
  В модуле реального времени стоит батарейка, которая продолжает отсчёт времени после выключения/сброса питания
  Как настроить время на часах. У нас есть возможность автоматически установить время на время загрузки прошивки, поэтому:
	- Ставим настройку RESET_CLOCK на 1
  - Прошиваемся
  - Сразу ставим RESET_CLOCK 0
  - И прошиваемся ещё раз
  - Всё
*/
/* Версия 1.6
  Настройка:
    Долгое нажатие на вторую кнопку переводит в режим корректировки времени/даты. Короткие нажатия первой/второй кнопок меняют цифру, 
    которая мигает в данный момент. Последовательный переход часы-минуты-год-месяц-день также по долгому нажатию на вторую кнопку.
    Долгое нажатие на первую кнопку во всех режимах кроме главного экрана переводит на главный экран.
    Долгое нажатие на первую кнопку в главном экране переводит в режим подготовки к автокалибровке датчика СО2 (начинает мигать 
    надпись СО2 и значение ppm). Долгое нажатие на вторую кнопку в этом режиме запускает автокалибровку, возврат по долгому нажатию 
    первой кнопки или таймеру бездействия.
*/
/* Версия 1.5
  - Добавлено управление яркостью
  - Яркость дисплея и светодиода СО2 меняется на максимальную и минимальную в зависимости от сигнала с фоторезистора
  Подключите датчик (фоторезистор) по схеме. Теперь на экране отладки справа на второй строчке появится величина сигнала
  с фоторезистора.
*/

// ------------------------- НАСТРОЙКИ -------------------- ntp1.stratum1.ru
#define RESET_CLOCK 0       // сброс часов на время загрузки прошивки (для модуля с несъёмной батарейкой). Не забудь поставить 0 и прошить ещё раз!
#define SENS_TIME 15000     // время обновления показаний сенсоров на экране, миллисекунд

// управление яркостью
#define BRIGHT_CONTROL 1      // 0/1 - запретить/разрешить управление яркостью (при отключении яркость всегда будет макс.)
#define BLUE_YELLOW 0       // жёлтый цвет вместо синего (1 да, 0 нет) но из за особенностей подключения жёлтый не такой яркий
#define DISP_MODE 2         // в правом верхнем углу отображать: 0 - год, 1 - день недели, 2 - секунды
#define WEEK_LANG 0         // язык дня недели: 0 - английский, 1 - русский (транслит)
#define DEBUG 0             // вывод на дисплей лог инициализации датчиков при запуске. Для дисплея 1602 не работает! Но дублируется через порт!
#define PRESSURE 0          // 0 - график давления, 1 - график прогноза дождя (вместо давления). Не забудь поправить пределы гроафика
#define CO2_SENSOR 1        // включить или выключить поддержку/вывод с датчика СО2 (1 вкл, 0 выкл)
#define DISPLAY_TYPE 1      // тип дисплея: 1 - 2004 (большой), 0 - 1602 (маленький)
#define DISPLAY_ADDR 0x27   // адрес платы дисплея: 0x27 или 0x3f. Если дисплей не работает - смени адрес! На самом дисплее адрес не указан

// пределы отображения для графиков
#define TEMP_MIN 15
#define TEMP_MAX 35
#define HUM_MIN 20
#define HUM_MAX 100
#define PRESS_MIN -100
#define PRESS_MAX 100
#define CO2_MIN 400
#define CO2_MAX 2400

// адрес BME280 жёстко задан в файле библиотеки Adafruit_BME280.h
// стоковый адрес был 0x77, у китайского модуля адрес 0x76.
// Так что если юзаете НЕ библиотеку из архива - не забудьте поменять

// если дисплей не заводится - поменяйте адрес (строка 54)

// пины
#define BACKLIGHT 10	// пин подсветки дисплея
#define PHOTO A3		// пин фоторезистора
#define DHTPIN 11   // пин для датчика DHT22

#define MHZ_RX 12   // RX датчика CO2
#define MHZ_TX 13   // TX датчика CO2

// режимы отображения
#define M_MAIN 0      // часы и данные
#define M_PLOT_TH 1   // график температуры за час
#define M_PLOT_TD 2   // график температуры за сутки
#define M_PLOT_HH 3   // график влажности за час
#define M_PLOT_HD 4   // график влажности за сутки
#define M_PLOT_PH 5   // график давления за час
#define M_PLOT_PD 6   // график давления за сутки
#define M_PLOT_CH 7   // график углекислого за час
#define M_PLOT_CD 8   // график углекислого за сутки
// режим настроек
#define M_SETUP 10    // настройки
#define M_SET_SC 11   // обнуление секунд
#define M_SET_HR 12   // настройка часов
#define M_SET_MN 13   // настройка минут
#define M_SET_YR 14   // настройка года
#define M_SET_MT 15   // настройка месяца
#define M_SET_DY 16   // настройка дня

// библиотеки
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if (DISPLAY_TYPE == 1)
LiquidCrystal_I2C lcd(DISPLAY_ADDR, 20, 4);
#else
LiquidCrystal_I2C lcd(DISPLAY_ADDR, 16, 2);
#endif

#include "RTClib.h"
RTC_DS3231 rtc;
DateTime now;

#include "DHT.h"
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
#include "Adafruit_BMP085.h"
#define SEALEVELPRESSURE_HPA (1013.25)
// ========================ТИП ДАТЧИКА DHT=============================================
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// ====================================================================================
DHT dht(DHTPIN, DHTTYPE);
//Adafruit_BME280 bme;
Adafruit_BMP085 bmp;

#if (CO2_SENSOR == 1)
#include <MHZ19_uart.h>
MHZ19_uart mhz19;
#endif

#include <GyverTimer.h>
GTimer_ms sensorsTimer(SENS_TIME);
GTimer_ms drawSensorsTimer(SENS_TIME);
GTimer_ms clockTimer(500);
GTimer_ms hourPlotTimer((long)4 * 60 * 1000);         // 4 минуты
GTimer_ms dayPlotTimer((long)1.6 * 60 * 60 * 1000);   // 1.6 часа
GTimer_ms plotTimer(240000);
GTimer_ms predictTimer((long)10 * 60 * 1000);         // 10 минут
GTimer_ms brightTimer(2000);
GTimer_ms idleTimer(5000);                           // время бездействия 10 с

int8_t hrs, mins, secs;
byte mode = M_MAIN;

#include "graph.h"
#include "LEDcontrol.h"
#include "bright.h"
#include "functions.h"
#include "buttons.h"

void setup() {
  Serial.begin(9600);

  pinMode(BACKLIGHT, OUTPUT);
  pinMode(LED_COM, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  setLED(0);

  digitalWrite(LED_COM, LED_MODE);
  analogWrite(BACKLIGHT, LCD_BRIGHT_MAX);

  lcd.init();
  lcd.backlight();
  lcd.clear();

#if (DEBUG == 1 && DISPLAY_TYPE == 1)
  boolean status = true;

  setLED(0);

#if (CO2_SENSOR == 1)
  lcd.setCursor(0, 0);
  lcd.print(F("MHZ-19... "));
  Serial.print(F("MHZ-19... "));
  mhz19.begin(MHZ_TX, MHZ_RX);
  mhz19.setAutoCalibration(false);
  mhz19.setRange5000();
  mhz19.getStatus();    // первый запрос, в любом случае возвращает -1
  delay(500);
  if (mhz19.getStatus() == 0) {
    lcd.print(F("OK"));
    Serial.println(F("OK"));
  } else {
    lcd.print(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }
#endif

  setLED(2);
  lcd.setCursor(0, 1);
  lcd.print(F("RTC... "));
  Serial.print(F("RTC... "));
  delay(50);
  if (rtc.begin()) {
    lcd.print(F("OK"));
    Serial.println(F("OK"));
  } else {
    lcd.print(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }

  setLED(3);
  lcd.setCursor(0, 2);
  lcd.print(F("BMP085... "));
  Serial.print(F("BMP085... "));
  delay(50);
  if (bmp.begin()) {
    lcd.print(F("OK"));
    Serial.println(F("OK"));
  } else {
    lcd.print(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }

  setLED(0);
  lcd.setCursor(0, 3);
  if (status) {
    setLED(2);
    lcd.print(F("All good"));
    Serial.println(F("All good"));
  } else {
    setLED(1);
    lcd.print(F("Check wires!"));
    Serial.println(F("Check wires!"));
    while (1) {
      setLED(1);
      lcd.setCursor(14, 1);
      lcd.print("P:    ");
      lcd.setCursor(16, 1);
      lcd.print(analogRead(PHOTO), 1);
      Serial.println(analogRead(PHOTO));
      delay(500);
      setLED(0);
      delay(500);
    }
  }
#else

#if (CO2_SENSOR == 1)
  mhz19.begin(MHZ_TX, MHZ_RX);
  mhz19.setAutoCalibration(false);
#endif
  rtc.begin();
  bmp.begin();
  dht.begin();
#endif

/*  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   ); */

  if (RESET_CLOCK || rtc.lostPower())
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //TimeQuery();

  mode = M_MAIN;

  //bme.takeForcedMeasurement();
  uint32_t Pressure = bmp.readPressure();
  for (byte i = 0; i < 6; i++) {   // счётчик от 0 до 5
    pressure_array[i] = Pressure;  // забить весь массив текущим давлением
    time_array[i] = i;             // забить массив времени числами 0 - 5
  }

  if (DISPLAY_TYPE == 1) {
    //loadClock();
    //drawClock(hrs, mins, 0, 0);
    //drawDate();
    readSensors();
    now = rtc.now();
    resetMode();
  } else {
    readSensors();
    drawSensors();
  }
}

void loop() {
  if (brightTimer.isReady()) checkBrightness();     // яркость
  if (sensorsTimer.isReady()) readSensors();        // читаем показания датчиков с периодом SENS_TIME

#if (DISPLAY_TYPE == 1)
  if (clockTimer.isReady()) clockTick();            // два раза в секунду пересчитываем время и мигаем точками
  plotSensorsTick();                                // тут внутри несколько таймеров для пересчёта графиков (за час, за день и прогноз)
  modesTick();                                      // тут ловим нажатия на кнопку и переключаем режимы
  if (mode == M_MAIN) {                             // в режиме "главного экрана"
    if (drawSensorsTimer.isReady()) drawSensors();  // обновляем показания датчиков на дисплее с периодом SENS_TIME
  } else if (mode > M_SETUP) {                      // настройка времени
    LEDrun();                                       // гирлянда
  } else {                                          // в любом из графиков
    if (plotTimer.isReady()) redrawPlot();          // перерисовываем график
  }
#else
  if (drawSensorsTimer.isReady()) drawSensors();
#endif
}