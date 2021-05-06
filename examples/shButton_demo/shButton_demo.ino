#include <shButton.h>

// демонстрация основных возможностей для работы с тактовой кнопкой

/*
 инициализация кнопки, параметры:
* пин, к которому подключена кнопка;
* режим подключения кнопки: 
	PULL_UP (значение по умолчанию) - кнопка подтянута к VCC - (pin -> кнопка -> GND); 
	PULL_DOWN - кнопка подтянута к GND - (pin -> кнопка -> VCC;
* тип кнопки: 
	BTN_NO - с нормально разомкнутыми контактами (по умолчанию);
	BTN_NC - с нормально замкнутыми контактами);
*/
shButton but(10); // равнозначно shButton but(10, PULL_UP, BTN_NO)

void setup()
{
  // режим пина кнопки устанавливается автоматически

  // необязательные установки
  but.setDebounce(80); // установка времени антидребезга, мс (по умолчанию 50 мс) 
  but.setTimeout(800); // установка времени удержания кнопки нажатой, мс (по умолчанию 500 мс)

  Serial.begin(9600);
}

void loop()
{
  static int8_t oldState = -1;
  // опрос кнопки, для уверенной обработки состояния кнопки опросы нужно делать как можно чаще
  int8_t curState = but.getButtonState();
  if (oldState != curState)
  {
    oldState = curState;
    switch (curState)
    {
      case BTN_RELEASED:
      //  Serial.println("button released");
        break;
      case BTN_UP:
        Serial.println("button up");
        break;
      case BTN_PRESSED:
      //  Serial.println("button pressed");
        break;
      case BTN_LONGCLICK :
        Serial.println("button hold");
        break;
      case BTN_DOWN:
        Serial.println("button down");
        break;
      case BTN_DBLCLICK:
        Serial.println("button dblclick");
        break;
    }
  }
}
