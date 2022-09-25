#include "GyverFilters.h"

#define SENS_BRIGHT_MIN 55    // минимальная величина сигнала (0-1023)
#define SENS_BRIGHT_MAX 750   // максимальная величина сигнала (0-1023)
#define LCD_BRIGHT_MAX 210    // макс яркость подсветки дисплея (0 - 255)
#define LCD_BRIGHT_MIN 5      // мин яркость подсветки дисплея (0 - 255)

GKalman brightFilter(25, 25, 0.05);

void showBright(int* valBright) {
  lcd.setCursor(17, 3);
  if (*valBright < 100) lcd.write(32);
  if (*valBright < 10) lcd.write(32);
  lcd.print(*valBright);
}

void checkBrightness() {
  int LIMIT_BRIGHT, LCD_BRIGHT, LED_BRIGHT;
  LIMIT_BRIGHT = brightFilter.filtered(constrain(analogRead(PHOTO), SENS_BRIGHT_MIN, SENS_BRIGHT_MAX));
  LCD_BRIGHT = map(LIMIT_BRIGHT, SENS_BRIGHT_MIN, SENS_BRIGHT_MAX, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX);
  LED_BRIGHT = map(LIMIT_BRIGHT, SENS_BRIGHT_MIN, SENS_BRIGHT_MAX, LED_BRIGHT_MIN, LED_BRIGHT_MAX);
  showBright(&LIMIT_BRIGHT);
  analogWrite(BACKLIGHT, LCD_BRIGHT);
#if (LED_MODE == 0)
    LED_ON = (LED_BRIGHT);
#else
    LED_ON = (255 - LED_BRIGHT);
#endif
}

