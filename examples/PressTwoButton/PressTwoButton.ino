/**
 * @file PressTwoButton.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief 
 * @version 2.0
 * @date 22.08.2022
 * 
 * @copyright Copyright (c) 2022
 * 
 * Пример обработки одновременного нажатия на две кнопки
 */
#include <shButton.h>

#define BTN1_PIN 10 // пин, к которому подключена первая кнопка
#define BTN2_PIN 11 // пин, к которому подключена вторая кнопка

/*
 инициализация кнопок, параметры:
* пин, к которому подключена кнопка;
* режим подключения кнопки: 
	PULL_UP (значение по умолчанию) - кнопка подтянута к VCC - (pin -> кнопка -> GND); 
	PULL_DOWN - кнопка подтянута к GND - (pin -> кнопка -> VCC;
* тип кнопки: 
	BTN_NO - с нормально разомкнутыми контактами (по умолчанию);
	BTN_NC - с нормально замкнутыми контактами);
*/
shButton but_1(BTN1_PIN); // равнозначно shButton but_1(BTN1_PIN, PULL_UP, BTN_NO)
shButton but_2(BTN2_PIN);

void setup()
{
  // режим пина кнопок устанавливается автоматически

  but_1.setVirtualClickOn(); // включение режима виртуального клика
  but_2.setVirtualClickOn();

  Serial.begin(9600);
}

void loop()
{
  but_1.getButtonState();
  but_2.getButtonState();
  if (but_1.isSecondButtonPressed(but_2) || but_2.isSecondButtonPressed(but_1))
  { // действие по одновременно нажатым двум кнопкам
    Serial.println("Two buttons pressed");
  }
  else
  {
    switch (but_1.getLastState()) // кнопка 1
    {
    case BTN_ONECLICK:
      if (!but_2.isButtonClosed())
      { // действие по нажатию первой кнопки
        Serial.println("Button No1 pressed");
      }
      break;
    }

    switch (but_2.getLastState()) // кнопка 2
    {
    case BTN_ONECLICK:
      if (!but_1.isButtonClosed())
      { // действие по нажатию второй кнопки
        Serial.println("Button No2 pressed");
      }
      break;
    }
  }
}