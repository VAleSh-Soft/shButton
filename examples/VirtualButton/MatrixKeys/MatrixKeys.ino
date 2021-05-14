#include <shButton.h>

// пример работы с виртуальными кнопками на примере матричной клавиатуры 3х4

const int Rows[] = {11, 10, 9, 8}; // пины строк
const int Cols[] = {7, 6, 5};      // пины столбцов

/* инициализация кнопок
  для виртуальной кнопки не нужно указывать никаких параметров, т.к. 
  при работе с ней положение контактов кнопки определяется не 
  библиотекой, а внешним кодом
*/
shButton btn_1;
shButton btn_2;
shButton btn_3;
shButton btn_4;
shButton btn_5;
shButton btn_6;
shButton btn_7;
shButton btn_8;
shButton btn_9;
shButton btn_s;
shButton btn_0;
shButton btn_g;

// объединяем кнопки в массив для удобства работы
shButton btnList[4][3] =
{
  {btn_1, btn_2, btn_3},
  {btn_4, btn_5, btn_6},
  {btn_7, btn_8, btn_9},
  {btn_s, btn_0, btn_g}
};

// создаем массив символов, привязанных к кнопкам клавиатуры
char charList[4][3] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {  // выставляем пины строк на выход
    pinMode(Rows[i], OUTPUT);
    digitalWrite(Rows[i], HIGH);
  }
  for (int i = 0; i < 3; i++) {  // выставляем пины столбцов на вход
    pinMode(Cols[i], INPUT_PULLUP);
  }
}

void loop()
{
  // опрос кнопок, для уверенной обработки состояния кнопок опросы нужно делать как можно чаще
  for (byte r = 0; r < 4; r++)
  {
    digitalWrite(Rows[r], LOW);     // последовательно выставляем по одной строке в LOW
    for (byte c = 0; c < 3; c++) // и считываем столбцы, определяя состояние кнопок
    {
      switch (btnList[r][c].getButtonState(!digitalRead(Cols[c])))
      { case BTN_DOWN:
          Serial.println(charList[r][c]);
          break;
      }
    }
    digitalWrite(Rows[r], HIGH);       // возвращем строку в HIGH и крутим дальше
  }
}