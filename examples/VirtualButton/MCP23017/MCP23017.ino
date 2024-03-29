/**
 * @file MCP23017.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief 
 * @version 1.0
 * @date 14.05.2021
 * 
 * @copyright Copyright (c) 2022
 * 
 * пример работы с виртуальными кнопками на примере расширителя портов MCP23017
 */
#include <Wire.h>
#include <Adafruit_MCP23017.h> // https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include <shButton.h>

/* инициализация кнопок
  для виртуальной кнопки не нужно указывать никаких параметров, т.к. 
  при работе с ней положение контактов кнопки определяется не 
  библиотекой, а внешним кодом
*/
shButton but_0;
shButton but_1;
shButton but_2;

// инициализация расширителя портов (схему подключения см. в даташите расширителя портов)
Adafruit_MCP23017 mcp;

// объединяем кнопки в массив для удобства работы
shButton btnList[] = {but_0, but_1, but_2};

void setup()
{
  Serial.begin(9600);
  mcp.begin(); // используется адрес по умолчанию - 0
  // настраиваем пины расширителя и при необходимости настраиваем кнопки
  for (uint8_t i = 0; i < 3; i++)
  {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);        // включение подтяжки к VCC внутренними резисторами 100K
    btnList[i].setDebounce(80); // установка времени антидребезга, мс (по умолчанию 50 мс)
  }
}

void loop()
{
  // опрос кнопок, для уверенной обработки состояния кнопок опросы нужно делать как можно чаще
  for (uint8_t i = 0; i < 3; i++)
  {
    switch (btnList[i].getButtonState(!mcp.digitalRead(i)))
    {
    case BTN_DOWN:
      Serial.print("DOWN - but_");
      Serial.println(i);
      mcp.digitalWrite((i) + 7, !mcp.digitalRead(i));
      break;
    case BTN_UP:
      Serial.print("UP - but_");
      Serial.println(i);
      mcp.digitalWrite((i) + 7, !mcp.digitalRead(i));
      break;
    }
  }
}