#define LED_MODE 0          // тип RGB светодиода: 0 - главный катод, 1 - главный анод

#define LED_COM 7		// общий
#define LED_R 9			// красный
#define LED_G 6			// зелёный
#define LED_B 5			// синий (жёлтый)

#define LED_BRIGHT_MAX 180    // макс яркость светодиода СО2 (0 - 255)
#define LED_BRIGHT_MIN 10     // мин яркость светодиода СО2 (0 - 255)

#if (LED_MODE == 0)
byte LED_ON = (LED_BRIGHT_MAX);
byte LED_OFF = (0);
#else
byte LED_ON = (255 - LED_BRIGHT_MAX);
byte LED_OFF = (255);
#endif

void setLED(byte color) { //0x00000BGR
  switch (color) {    // 0 выкл, 1 красный, 2 зелёный, 3 синий (или жёлтый)
    case 0:
      analogWrite(LED_R, LED_OFF);
      analogWrite(LED_G, LED_OFF);
      analogWrite(LED_B, LED_OFF);
      break;
    case 1: // красный
      analogWrite(LED_R, LED_ON);
      analogWrite(LED_G, LED_OFF);
      analogWrite(LED_B, LED_OFF);
      break;
    case 2: // зелёный
      analogWrite(LED_G, LED_ON);
      analogWrite(LED_R, LED_OFF);
      analogWrite(LED_B, LED_OFF);
      break;
    case 3: // жёлтый
      analogWrite(LED_R, LED_ON / 2 );    // вдвое уменьшаем красный
      analogWrite(LED_G, LED_ON / 2);   // чутка уменьшаем зелёный
      analogWrite(LED_B, LED_OFF); // при общем резисторе само уменьшится ))
      break;
    case 4: // синий
      analogWrite(LED_R, LED_OFF); 
      analogWrite(LED_G, LED_OFF);   
      analogWrite(LED_B, LED_ON);
      break;
  }
}

void LEDrun() {
  static uint8_t led_mode = 1;
  static boolean oldFlag = dotFlag;
  if (oldFlag != dotFlag) {
    led_mode <<= 1;
    if (led_mode > 4) led_mode = 1;
  }
  setLED(led_mode);
}