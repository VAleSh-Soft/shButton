#include <shButton.h>

// демонстрация основных возможностей для работы с тактовой кнопкой

#define BTN_PIN 10 // пин, к которому подключена кнопка

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
shButton but(BTN_PIN); // равнозначно shButton but(BTN_PIN, PULL_UP, BTN_NO)

void setup()
{
  // режим пина кнопки устанавливается автоматически

  // необязательные установки
  but.setTimeoutOfDebounce(80);   // установка времени антидребезга, мс (по умолчанию 50 мс)
  but.setTimeoutOfLongClick(800); // установка времени удержания кнопки нажатой, мс (по умолчанию 500 мс)

  Serial.begin(9600);
}

void loop()
{
  // опрос кнопки, для уверенной обработки состояния кнопки опросы нужно делать как можно чаще
  switch (but.getButtonState())
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
  case BTN_LONGCLICK:
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
