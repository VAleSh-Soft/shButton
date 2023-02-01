/**
 * @file UseButtonFlag.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief
 * @version 1.0
 * @date 01.02.2023
 *
 * @copyright Copyright (c) 2023
 *
 * демонстрация работы с флагом кнопки
 */
#define USE_BUTTON_FLAG // объявляем, что намерены использовать флаги кнопок; обязательно задавать перед объявлением файла shButton.h
#include <shButton.h>

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
shButton btn(BTN_PIN); // равнозначно shButton btn(BTN_PIN, PULL_UP, BTN_NO)

// пользовательские флаги для использования с кнопкой; назначаются в соответствие с логикой вашей программы
const uint8_t BTN_FLAG_NONE = 0;
const uint8_t BTN_FLAG_OK = 1;
const uint8_t BTN_FLAG_DK = 2;
const uint8_t BTN_FLAG_LK = 3;

// функция считывания состояния кнопки
void checkButton()
{
  switch (btn.getButtonState())
  {
  case BTN_ONECLICK:
    btn.setButtonFlag(BTN_FLAG_OK);
    break;
  case BTN_DBLCLICK:
    btn.setButtonFlag(BTN_FLAG_DK);
    break;
  case BTN_LONGCLICK:
    btn.setButtonFlag(BTN_FLAG_LK);
    break;
  }
}

// основная функция
void general_func()
{
  switch (btn.getButtonFlag(true))
  {
  case BTN_FLAG_OK:
    Serial.println("button oneclick");
    break;
  case BTN_FLAG_DK:
    Serial.println("button dblclick");
    break;
  case BTN_FLAG_LK:
    Serial.println("button hold");
    break;
  }
}

void setup()
{
  btn.setVirtualClickOn();            // включаем режим виртуального клика, т.к. будем нам нужно отрабатывать реакцию на одиночный, двойной клик и удержание кнопки нажатой
  btn.setLongClickMode(LCM_ONLYONCE); // включаем режим одиночного события при удержании кнопки

  Serial.begin(9600);
}

void loop()
{
  // опрос кнопки, для уверенной обработки состояния кнопки опросы нужно делать как можно чаще
  checkButton();

  static uint32_t timer = 0;
  if (millis() - timer >= 100)
  {
    timer = millis();
    // основная функция запускается через каждые 100 милисекунд
    general_func();
  }
}
