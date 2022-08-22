#include <shButton.h>

// Пример раздельной реакции на одинарный и двойной клик кнопкой с использованием режима виртуального клика

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

  but.setVirtualClickOn(); // включение режима виртуального клика

  Serial.begin(9600);
}

void loop()
{

  switch (but.getButtonState())
  {
  case BTN_ONECLICK:
    // действие по одинарному клику 
    Serial.println("onClick");
    break;
  case BTN_DBLCLICK:
    // действие по двойному клику
    Serial.println("onDblClick");
    break;
  }
}
