#include <shButton.h>

// Пример обработки удержания кнопки нажатой - получение одиночного события или серии событий, пока кнопка нажата

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
shButton but_1(10); // равнозначно shButton but_1(10, PULL_UP, BTN_NO)
shButton but_2(11);

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