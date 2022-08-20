#include <shButton.h>

// пример работы с виртуальными кнопками на примере матричной клавиатуры 4х3

#define rows 4 // количество строк
#define cols 3 // количество столбцов

const uint8_t rowsPin[] = {11, 10, 9, 8}; // пины строк
const uint8_t colsPin[] = {7, 6, 5};      // пины столбцов

/* инициализация кнопок
  для виртуальной кнопки не нужно указывать никаких параметров, т.к. 
  при работе с ней положение контактов кнопки определяется не 
  библиотекой, а внешним кодом
*/
shButton but_1;
shButton but_2;
shButton but_3;
shButton but_4;
shButton but_5;
shButton but_6;
shButton but_7;
shButton but_8;
shButton but_9;
shButton but_s;
shButton but_0;
shButton but_g;

// объединяем кнопки в массив для удобства работы
shButton btnList[rows][cols] =
    {
        {but_1, but_2, but_3},
        {but_4, but_5, but_6},
        {but_7, but_8, but_9},
        {but_s, but_0, but_g}};

// создаем массив символов, привязанных к кнопкам клавиатуры
char charList[rows][cols] =
    {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}};

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < rows; i++)
  { // выставляем пины строк на выход
    pinMode(rowsPin[i], OUTPUT);
    digitalWrite(rowsPin[i], HIGH);
  }
  for (int i = 0; i < cols; i++)
  { // выставляем пины столбцов на вход
    pinMode(colsPin[i], INPUT_PULLUP);
  }
}

void loop()
{
  // опрос кнопок, для уверенной обработки состояния кнопок опросы нужно делать как можно чаще
  for (uint8_t r = 0; r < rows; r++)
  {
    digitalWrite(rowsPin[r], LOW);  // последовательно выставляем по одной строке в LOW
    for (uint8_t c = 0; c < cols; c++) // и считываем столбцы, определяя состояние кнопок
    {
      switch (btnList[r][c].getButtonState(!digitalRead(colsPin[c])))
      {
      case BTN_DOWN:
        Serial.println(charList[r][c]);
        break;
      }
    }
    digitalWrite(rowsPin[r], HIGH); // возвращем строку в HIGH и крутим дальше
  }
}