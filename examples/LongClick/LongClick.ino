/**
 * @file LongClick.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief 
 * @version 1.0
 * @date 16.09.2020
 * 
 * @copyright Copyright (c) 2022
 * 
 * Пример обработки удержания кнопки нажатой - получение одиночного 
 * события или серии событий, пока кнопка нажата
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

  but_1.setLongClickMode(LCM_ONLYONCE);    // включение однократного события при удержании кнопки
  but_2.setLongClickMode(LCM_CLICKSERIES); // включение серии событий при удержании кнопки

  Serial.begin(9600);
}

void loop()
{
  static word count = 0;
  switch (but_1.getButtonState())
  {
  case BTN_LONGCLICK:
    // действие по одиночному событию
    Serial.println("Only Once LongClickEvent");
    break;
  }
  switch (but_2.getButtonState())
  {
  case BTN_DOWN:
    // обнуление счетчика
    count = 0;
    break;
  case BTN_LONGCLICK:
    // действие по серийному событию 
    Serial.print(++count);
    Serial.println(" - LongClickEventSeries");
    break;
  }
}