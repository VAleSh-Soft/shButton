#include <shButton.h>

// Пример работы с виртуальными кнопками на примере аналоговой клавиатуры

// аналоговый пин, к которому подключена клавиатура
#define BTN_PIN A0
// значения, получаемые функцией analogRead() при нажатии на кнопки; зависят от схемы подключения и номиналов используемых резисторов;
#define BTN_1_LVL 20
#define BTN_2_LVL 512
#define BTN_3_LVL 680
// допуск на погрешность измерения
#define LVL_LIMIT 20

/* инициализация кнопок
  для виртуальной кнопки не нужно указывать никаких параметров, т.к. 
  при работе с ней положение контактов кнопки определяется не 
  библиотекой, а внешним кодом
*/
shButton but_1;
shButton but_2;
shButton but_3;

// определение положения контактов кнопки - замкнуты/не замкнуты; в функцию передается значение, полученное функцией analogRead() и пороговый уровень соответствующей кнопки - BTN_1_LVL и т.д.
bool isButtonClosed(word lvl, word btn_lvl)
{
  return ((lvl >= btn_lvl - LVL_LIMIT) && (lvl <= btn_lvl + LVL_LIMIT));
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // опрос кнопок, для уверенной обработки состояния кнопок опросы нужно делать как можно чаще
  word res = analogRead(BTN_PIN);
  switch (but_1.getButtonState(isButtonClosed(res, BTN_1_LVL)))
  {
  case BTN_DOWN:
    Serial.println("btn1_Down");
    break;
  case BTN_UP:
    Serial.println("btn1_Up");
    break;
  }
  switch (but_2.getButtonState(isButtonClosed(res, BTN_2_LVL)))
  {
  case BTN_DOWN:
    Serial.println("btn2_Down");
    break;
  case BTN_UP:
    Serial.println("btn2_Up");
    break;
  }
  switch (but_3.getButtonState(isButtonClosed(res, BTN_3_LVL)))
  {
  case BTN_DOWN:
    Serial.println("btn3_Down");
    break;
  case BTN_UP:
    Serial.println("btn3_Up");
    break;
  }
}
