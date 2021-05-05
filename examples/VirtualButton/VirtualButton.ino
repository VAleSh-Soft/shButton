#include <shButton.h>

// Пример работы с виртуальными кнопками на примере аналоговой клавиатуры

// аналоговый пин, к которому подключена клавиатура
#define BTN_PIN A0
// значения, получаемые функцией analogRead() при нажатии на кнопки; зависят от схемы подключения и номиналов используемых резисторов;
#define BTN_1_LVL 512
#define BTN_2_LVL 768
// допуск на погрешность измерения
#define LVL_LIMIT 20

/* инициализация кнопок.
для виртуальной кнопки не нужно указывать никаких параметров, т.к. при работе с ней положение контактов кнопки определяется не библиотекой, а внешним кодом
*/
shButton but_1();
shButton but_2();

// определение положения контактов кнопки - замкнуты/не замкнуты
bool isButtonClosed(word lvl)
{
  word res = analogRead(BTN_PIN);
  return ((res >= lvl - LVL_LIMIT) && (res <= lvl + LVL_LIMIT))
}

setup()
{
  Serial.begin(9600);
}

loop()
{
  // опрос кнопок, для уверенной обработки состояния кнопок опросы нужно делать как можно чаще
  switch (but_1.getButtonState(isButtonClosed(BTN_1_LVL)))
  {
  case BTN_DOWN:
    Serial.println("btn1_Down");
    break;
  case BTN_UP:
    Serial.println("btn1_Up");
    break;
  }
  switch (but_2.getButtonState(isButtonClosed(BTN_2_LVL)))
  {
  case BTN_DOWN:
    Serial.println("btn2_Down");
    break;
  case BTN_UP:
    Serial.println("btn2_Up");
    break;
  }
}
