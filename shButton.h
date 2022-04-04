#pragma once
#include <Arduino.h>

// флаги свойств и состояния кнопки - биты поля _flags
#define FLAG_BIT 0          // сохраненное состояние кнопки - нажата/не нажата
#define INPUTTYPE_BIT 1     // тип подключения - PULL_UP/ PULL_DOWN
#define BTNTYPE_BIT 2       // тип кнопки - нормально разомкнутая или нормально замкнутая
#define DEBOUNCE_BIT 3      // флаг включения подавления дребезга
#define VIRTUALCLICK_BIT 4  // режим виртуального клика
#define VIRTUALBUTTON_BIT 5 // флаг режима виртуальной кнопки
#define ONECLICK_BIT 6      // флаг одиночного клика
#define LONGCLICK_BIT 7     // флаг длинного клика

// значения по умолчанию
#define DEBOUNCE_TIMEOUT 50
#define LONGCLICK_TIMEOUT 500
#define DBLCLICK_TIMEOUT 300
#define INTERVAL_OF_SERIAL 200
#define NO_PIN 255

// типы кнопки
#define BTN_NO 0 // кнопка с нормально разомкнутыми контактами
#define BTN_NC 1 // кнопка с нормально замкнутыми контактами

// типы подключения кнопки
#define PULL_UP 0   // кнопка подтянута к VCC
#define PULL_DOWN 1 // кнопка подтянута к GND

// состояние кнопки
#define BTN_RELEASED 0 // кнопка отпущена
#define BTN_PRESSED 1  // кнопка нажата, но время удержания не вышло

// события кнопки
#define BTN_UP 2       // кнопка только что отпущена
#define BTN_DOWN 3     // кнопка только что нажата
#define BTN_DBLCLICK 4 // двойной клик

// виртуальные события кнопки
#define BTN_ONECLICK 5  // одиночный клик, следует через некоторый интервал после нажатия кнопки, если за это время не последовал двойной клик или длительное удержание кнопки нажатой; по умолчанию событие отключено
#define BTN_LONGCLICK 6 // событие, соответствующее удержанию кнопки дольше времени удержания; может быть быть непрерывным (по умолчанию), однократным или следовать через определенные интервалы

// режимы отработки удержания кнопки
#define LCM_CONTINUED 0   // непрерывное событие, генерируется постоянно, пока кнопка удерживается нажатой, если интервал удержания превышен; значение по умолчанию
#define LCM_ONLYONCE 1    // однократное событие, генерируется только один раз по истечении интервала удержания кнопки
#define LCM_CLICKSERIES 2 // по истечении интервала удержания кнопки событие генерируется постоянно через равные интервалы времени

class shButton
{
private:
  byte _PIN = NO_PIN;                                // пин, на который посажена кнопка
  uint16_t _debounce_timeout = DEBOUNCE_TIMEOUT;     // интервал подавления дребезга контактов, мс
  uint16_t _longclick_timeout = LONGCLICK_TIMEOUT;   // интервал удержания кнопки нажатой, мс
  uint16_t _dblclck_timeout = DBLCLICK_TIMEOUT;      // интервал двойного клика, мс
  byte _longclick_mode = LCM_CONTINUED;              // режим удержания кнопки;
  uint16_t _interval_of_serial = INTERVAL_OF_SERIAL; // интервал следования события BTN_LONGCLICK, если установлен режим LCM_CLICKSERIES, мс
  byte _btn_state = BTN_RELEASED;                    // текущее состояние кнопки

  byte _flags = 0; // набор флагов свойств и состояния кнопки
  /*
* 0 бит - сохраненное состояние кнопки - нажата(1)/не нажата(0)
* 1 бит - тип подключения - PULL_UP(0)/PULL_DOWN(1)
* 2 бит - тип кнопки - нормально разомкнутая (BTN_NO(0)) или нормально замкнутая (BTN_NC(1))
* 3 бит - флаг включения подавления дребезга - пока флаг поднят (1), изменения состояния не принимаются
* 4 бит - режим виртуального клика, 0 - выключен, 1 - включен
* 5 бит - флаг виртуальной кнопки, 0 - обычная кнопка, 1 - виртуальная кнопка
* 6 бит - флаг одиночного клика, 0 - не было, 1 - был одиночный клик
* 7 бит - флаг длинного клика, 0 - не было, 1 - был длинный клик
  */

  uint32_t btn_timer = 0; // таймер отработки подавления дребезга контактов и длинного клика
  uint32_t dbl_timer = 0; // таймер двойного клика

  // получение мгновенного состояния кнопки - нажата/не нажата с учетом типа подключения и без учета дребезга контактов
  bool getButtonFlag();
  // установка кнопке состояния "только что нажата" или "только что отпущена"
  void setBtnUpDown(bool flag, uint32_t thisMls);
  // получение состояния бита
  bool getFlag(byte _bit);
  // установка состояния бита
  void setFlag(byte _bit, bool x);

public:
  // Варианты инициализации:
  // shButton btn(пин);	 - с привязкой к пину и без указания типа подключения (по умолч. PULL_UP) и типа контактов (по умолчанию BTN_NO - нормально разомкнутые контакты)
  // shButton btn(пин, тип подключения);	- с привязкой к пину и указанием типа подключения (PULL_UP / PULL_DOWN)
  // shButton btn(пин, тип подключения, тип кнопки);	- с привязкой к пину и указанием типа подключения (PULL_UP / PULL_DOWN) и типа кнопки (BTN_NO/BTN_NC)
  shButton(byte pin, byte inputtype = PULL_UP, byte btntype = BTN_NO);
  // shButton btn; - без привязки к конкретному пину и указания типов подключения и кнопки - режим виртуальной кнопки
  shButton();

  // получение состояния кнопки - отпущена/нажата/удерживается в режиме виртуальной кнопки
  // положение кнопки - нажата/не нажата для определения ее состояния
  byte getButtonState(bool isClosed);

  // получение состояния кнопки - отпущена/нажата/удерживается в режиме обычной кнопки
  byte getButtonState();

  // получение последнего состояния кнопки; запрашивает статус кнопки, определенный последним вызовом метода getButtonState()
  byte getLastState();

  // возвращает true, если контакты кнопки по результату последней проверки замкнуты; если toChecked == true будет выполнен опрос кнопки
  bool isButtonClosed(bool toChecked = false);

  // возвращает true, если в момент возникновения события BTN_DOWN текущей кнопки кнопка _but уже нажата
  bool isSecondButtonPressed(shButton &_but);

  // принудительный сброс состояния кнопки; может понадобиться, если по каким-то причинам нужно, например, исключить возникновение событий BTN_ONECLICK и BTN_DBLCLICK
  void resetButtonState();

  // установка типа подключения кнопки (PULL_UP - подтянута к VCC, PULL_DOWN - к GND)
  void setInputType(byte inputtype);

  // установка типа кнопки (BTN_NO - нормально разомкнутая, BTN_NC - нормально замкнутая)
  void setButtonType(byte btntype);

  // установка времени антидребезга (по умолчанию 50 мс); для отключения антидребезга нужно задать 0 мс
  void setTimeoutOfDebounce(uint16_t new_timeout);

  // установка таймаута удержания кнопки (по умолчанию 500 мс)
  void setTimeoutOfLongClick(uint16_t new_timeout);

  // установка интервала двойного клика (по умолчанию 300 мс)
  void setTimeoutOfDblClick(uint16_t new_timeout);

  // включение режима "Виртуальный клик"
  void setVirtualClickOn(bool virtualclick_on);

  // установка режима обработки удержания кнопки нажатой
  void setLongClickMode(byte longclickmode);

  // установка интервала выдачи события BTN_LONGCLICK в режиме LCM_CLICKSERIES
  void setIntervalOfSerial(uint16_t new_interval);

  // ==== deprecated ============================

  // метод устарел, используйте метод setIntervalOfSerial(uint16_t)
  [[deprecated("Используйте метод setIntervalOfSerial(uint16_t); Use setIntervalOfSerial(uint16_t) function")]] void setLongClickTimeout(uint16_t new_timeout);

  // метод устарел, используйте метод setTimeoutOfDblClick(uint16_t)
  [[deprecated("Используйте метод setTimeoutOfDblClick(uint16_t); Use the setTimeoutOfDblClick(uint16_t) function")]] void setDblClickTimeout(uint16_t new_timeout);

  // метод устарел, используйте метод setTimeoutOfLongClick(uint16_t)
  [[deprecated("Используйте метод setTimeoutOfLongClick(uint16_t); Use the setTimeoutOfLongClick(uint16_t) function")]] void setTimeout(uint16_t new_timeout);

  // метод устарел, используйте метод setTimeoutOfDebounce(uint16_t)
  [[deprecated("Используйте метод setTimeoutOfDebounce(uint16_t); Use the setTimeoutOfDebounce(uint16_t) function")]] void setDebounce(uint16_t debounce);
};
