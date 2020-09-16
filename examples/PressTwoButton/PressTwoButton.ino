#include <shButton.h>

// Пример обработки одновременного нажатия на две кнопки

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

  but_1.setVirtualClickOn(true); // включение режима виртуального клика
  but_2.setVirtualClickOn(true); 

  Serial.begin(9600);
}

void loop()
{
  byte _but_1 = but_1.getButtonState();
  byte _but_2 = but_2.getButtonState();
  if ((_but_1 == BTN_DOWN && but_2.isButtonClosed()) || (_but_2 == BTN_DOWN && but_1.isButtonClosed()))
  { // действие по одновременно нажатым двум кнопкам
    Serial.println("Two buttons pressed");
    // сбросить состояние обеих кнопок
    but_1.resetButtonState();
    but_2.resetButtonState();
  }
  else
  {
    switch (_but_1)
    { 
    case BTN_ONECLICK:
      if (!but_2.isButtonClosed())
      { // действие по нажатию первой кнопки
        Serial.println("Button No1 pressed");
      }
      break;
    }

    switch (_but_2) // кнопка "-"
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