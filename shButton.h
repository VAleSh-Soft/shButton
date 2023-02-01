// #pragma once
#ifndef SH_BUTTON_H
#define SH_BUTTON_H

#include <Arduino.h>

// #define USE_BUTTON_FLAG // использовать или нет флаг для передачи данных

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
#define BTN_DBLCLICK 4 // двойной клик - второе нажатие до истечения интервала двойного клика; если непрерывно щелкать кнопкой, будут поочередно выдаваться события BTN_DOWN и BTN_DBLCLICK

// виртуальные события кнопки
#define BTN_ONECLICK 5  // одиночный клик, следует через некоторый интервал после нажатия кнопки, если за это время не последовал двойной клик или длительное удержание кнопки нажатой; по умолчанию событие отключено
#define BTN_LONGCLICK 6 // событие, соответствующее удержанию кнопки дольше времени удержания; может быть быть непрерывным (по умолчанию), однократным или следовать через определенные интервалы

// режимы отработки удержания кнопки
#define LCM_CONTINUED 0   // непрерывное событие BTN_LONGCLICK, генерируется постоянно, пока кнопка удерживается нажатой, если интервал удержания превышен; значение по умолчанию
#define LCM_ONLYONCE 1    // однократное событие BTN_LONGCLICK, генерируется только один раз по истечении интервала удержания кнопки, после чего будет постоянно выдаваться BTN_PRESSED до тех пор, пока кнопка будет удерживаться нажатой
#define LCM_CLICKSERIES 2 // по истечении интервала удержания кнопки событие BTN_LONGCLICK генерируется постоянно через равные интервалы времени, в промежутках выдается BTN_PRESSED

class shButton
{
private:
  uint8_t _PIN = NO_PIN;                             // пин, на который посажена кнопка
  uint16_t _debounce_timeout = DEBOUNCE_TIMEOUT;     // интервал подавления дребезга контактов, мс
  uint16_t _longclick_timeout = LONGCLICK_TIMEOUT;   // интервал удержания кнопки нажатой, мс
  uint16_t _dblclck_timeout = DBLCLICK_TIMEOUT;      // интервал двойного клика, мс
  uint8_t _longclick_mode = LCM_CONTINUED;           // режим удержания кнопки;
  uint16_t _interval_of_serial = INTERVAL_OF_SERIAL; // интервал следования события BTN_LONGCLICK, если установлен режим LCM_CLICKSERIES, мс
  uint8_t _btn_state = BTN_RELEASED;                 // текущее состояние кнопки

  uint8_t _flags = 0; // набор флагов свойств и состояния кнопки
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
  bool getContactsState();
  // установка кнопке состояния "только что нажата" или "только что отпущена"
  void setBtnUpDown(bool flag, uint32_t thisMls);
  // получение состояния бита
  bool getFlag(uint8_t _bit);
  // установка состояния бита
  void setFlag(uint8_t _bit, bool x);

#ifdef USE_BUTTON_FLAG
  uint8_t btn_flag = 0;
#endif
public:
  /**
   * @brief конструктор кнопки;
   *
   * @param pin пин, к которому подключена кнопка;
   * @param inputtype тип подключения (PULL_UP / PULL_DOWN), т.е. с подтяжкой к VCC или к GND;
   * @param btntype тип кнопки (BTN_NO / BTN_NC), т.е. с нормально разомкнутыми или нормально замкнутыми контактами;
   */
  shButton(uint8_t pin, uint8_t inputtype = PULL_UP, uint8_t btntype = BTN_NO);

  /**
   * @brief конструктор виртуальной кнопки, без привязки к конкретному пину и указания типа подключения и типа кнопки;
   * использование - shButton btn;
   */
  shButton();

  /**
   * @brief получение состояния кнопки - отпущена/нажата/удерживается;
   *
   * @return uint8_t, текущее состояние кнопки (BTN_RELEASED .. BTN_LONGCLICK);
   */
  uint8_t getButtonState();

  /**
   * @brief получение состояния кнопки - отпущена/нажата/удерживается в режиме виртуальной кнопки;
   *
   * @param isClosed текущее положение контактов кнопки, определяется внешним кодом
   * @return uint8_t, текущее состояние кнопки (BTN_RELEASED .. BTN_LONGCLICK);
   */
  uint8_t getButtonState(bool isClosed);

  /**
   * @brief получение последнего состояния кнопки; возвращает статус кнопки по результатам последнего вызова метода getButtonState();
   *
   * @return uint8_t, текущее состояние кнопки (BTN_RELEASED .. BTN_LONGCLICK);
   */
  uint8_t getLastState();

  /**
   * @brief возвращает true, если по результатам последнего вызова метода getButtonState() кнопка нажата;
   *
   * @return true/false - кнопка нажата/не нажата;
   */
  bool isButtonClosed();

  /**
   * @brief определение одновременного нажатия двух кнопок; возвращает true, если в момент возникновения события BTN_DOWN текущей кнопки кнопка _but уже нажата; если метод вернул true, состояние обеих кнопок сбрасывается;
   *
   * @param _but вторая кнопка, состояние которой проверяется
   * @param btn_state событие, при возникновении которого проверяется состояние второй кнопки
   * @return true/false - кнопка _but уже нажата/не нажата;
   */
  bool isSecondButtonPressed(shButton &_but, byte btn_state = BTN_DOWN);

  /**
   * @brief принудительный сброс состояния кнопки; может понадобиться, если по каким-то причинам нужно, например, исключить возникновение событий BTN_ONECLICK и BTN_DBLCLICK;
   *
   */
  void resetButtonState();

  /**
   * @brief установка типа подключения кнопки;
   *
   * @param inputtype тип подключения кнопки (PULL_UP - подтянута к VCC, PULL_DOWN - к GND);
   */
  void setInputType(uint8_t inputtype);

  //
  /**
   * @brief установка типа кнопки;
   *
   * @param btntype тип кнопки (BTN_NO - нормально разомкнутая, BTN_NC - нормально замкнутая);
   */
  void setButtonType(uint8_t btntype);

  /**
   * @brief установка интервала подавления антидребезга (значение по умолчанию 50 мс); для отключения антидребезга нужно задать 0 мс;
   *
   * @param new_timeout новое значение интервала подавления антидребезга в милисекундах;
   */
  void setTimeoutOfDebounce(uint16_t new_timeout);

  /**
   * @brief установка интервала удержания кнопки (значение по умолчанию 500 мс);
   *
   * @param new_timeout новое значение интервала удержания кнопки в милисекундах;
   */
  void setTimeoutOfLongClick(uint16_t new_timeout);

  /**
   * @brief установка интервала двойного клика (значение по умолчанию 300 мс);
   *
   * @param new_timeout новое значение интервала двойного клика в милисекундах;
   */
  void setTimeoutOfDblClick(uint16_t new_timeout);

  /**
   * @brief включение режима "Виртуальный клик";
   *
   * @param virtualclick_on true - включение режима; false - отключение режима;
   */
  void setVirtualClickOn(bool virtualclick_on = true);

  /**
   * @brief установка режима обработки удержания кнопки нажатой
   *
   * @param longclickmode режим обработки удержания кнопки (LCM_CONTINUED, LCM_ONLYONCE или LCM_CLICKSERIES);
   */
  void setLongClickMode(uint8_t longclickmode);

  /**
   * @brief установка интервала выдачи события BTN_LONGCLICK в режиме LCM_CLICKSERIES (значение по умолчанию 200 мс); установка значения 0 переведет кнопку в режим LCM_ONLYONCE;
   *
   * @param new_interval новое значение интервала в милисекундах;
   */
  void setIntervalOfSerial(uint16_t new_interval);

  /**
   * @brief считать флаг кнопки
   *
   * @param _clear если true - после считывания установить флаг в 0
   * @return uint8_t
   */
  uint8_t getButtonFlag(bool _clear = false)
  {
#ifdef USE_BUTTON_FLAG
    uint8_t result = btn_flag;
    if (_clear)
    {
      btn_flag = 0;
    }
    return (result);
#else
    return (0);
#endif
  }
  /**
   * @brief установить флаг кнопки
   *
   * @param _flag устанавливаемое значение
   */
  void setButtonFlag(uint8_t _flag)
  {
#ifdef USE_BUTTON_FLAG
    btn_flag = _flag;
#endif
  }

  // ==== deprecated ============================

  // метод устарел, используйте метод setIntervalOfSerial(uint16_t)
  void setLongClickTimeout(uint16_t new_timeout) __attribute__((deprecated("Используйте метод setIntervalOfSerial(uint16_t); Use setIntervalOfSerial(uint16_t) function")))
  {
    setIntervalOfSerial(new_timeout);
  }

  // метод устарел, используйте метод setTimeoutOfDblClick(uint16_t)
  void setDblClickTimeout(uint16_t new_timeout) __attribute__((deprecated("Используйте метод setTimeoutOfDblClick(uint16_t); Use the setTimeoutOfDblClick(uint16_t) function")))
  {
    setTimeoutOfDblClick(new_timeout);
  }

  // метод устарел, используйте метод setTimeoutOfLongClick(uint16_t)
  void setTimeout(uint16_t new_timeout) __attribute__((deprecated("Используйте метод setTimeoutOfLongClick(uint16_t); Use the setTimeoutOfLongClick(uint16_t) function")))
  {
    setTimeoutOfLongClick(new_timeout);
  }

  // метод устарел, используйте метод setTimeoutOfDebounce(uint16_t)
  void setDebounce(uint16_t debounce) __attribute__((deprecated("Используйте метод setTimeoutOfDebounce(uint16_t); Use the setTimeoutOfDebounce(uint16_t) function")))
  {
    setTimeoutOfDebounce(debounce);
  }
};

#endif