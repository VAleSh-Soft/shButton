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

#if defined(USE_BUTTON_FLAG)
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

#if defined(USE_BUTTON_FLAG)
  /**
   * @brief считать флаг кнопки
   *
   * @param _clear если true - после считывания установить флаг в 0
   * @return uint8_t
   */
  uint8_t getButtonFlag(bool _clear = false);

  /**
   * @brief установить флаг кнопки
   *
   * @param _flag устанавливаемое значение
   */
  void setButtonFlag(uint8_t _flag);
#endif
};

// ==== public =======================================

shButton::shButton(uint8_t pin, uint8_t inputtype, uint8_t btntype)
{
  _PIN = pin;
  setInputType(inputtype);
  setButtonType(btntype);
}

shButton::shButton()
{
  setFlag(VIRTUALBUTTON_BIT, true);
}

uint8_t shButton::getButtonState(bool isClosed)
{
  uint32_t thisMls = millis();

  // если поднят флаг подавления дребезга и интервал еще не вышел, больше ничего не делать
  if (_debounce_timeout > 0 &&
      getFlag(DEBOUNCE_BIT) &&
      thisMls - btn_timer < _debounce_timeout)
  {
    return (_btn_state);
  }

  // состояние кнопки не изменилось с прошлого опроса
  if (isClosed == getFlag(FLAG_BIT))
  { // и не поднят флаг подавления дребезга
    if (!getFlag(DEBOUNCE_BIT))
    {
      if (!isClosed)
      { // кнопка находится в отжатом состоянии
        _btn_state = BTN_RELEASED;
        if (thisMls - dbl_timer > _dblclck_timeout)
        { // если период двойного клика закончился, проверить на виртуальный клик и сбросить флаг одиночного клика
          if (getFlag(VIRTUALCLICK_BIT) && getFlag(ONECLICK_BIT))
          {
            _btn_state = BTN_ONECLICK;
          }
          setFlag(ONECLICK_BIT, false);
          setFlag(LONGCLICK_BIT, false);
        }
      }
      else if (thisMls - btn_timer < _longclick_timeout && !getFlag(LONGCLICK_BIT))
      { // кнопка находится в нажатом состоянии, но время удержания еще не вышло, и события удержания еще не было
        _btn_state = BTN_PRESSED;
      }
      else // если кнопка удерживается нажатой дольше времени удержания, то дальше возможны варианты
      {    // если события удержания еще не было, то выдать его
        if (!getFlag(LONGCLICK_BIT))
        {
          if (_longclick_mode == LCM_CLICKSERIES)
          {
            btn_timer = thisMls;
          }
          setFlag(LONGCLICK_BIT, true);
          _btn_state = BTN_LONGCLICK;
        }
        else // если это уже не первое событие удержания, то дальше согласно выбранного режима
        {
          switch (_longclick_mode)
          {
          case LCM_ONLYONCE:
            _btn_state = BTN_PRESSED;
            break;
          case LCM_CLICKSERIES:
            if (thisMls - btn_timer >= _interval_of_serial)
            {
              btn_timer = thisMls;
              _btn_state = BTN_LONGCLICK;
            }
            else
            {
              _btn_state = BTN_PRESSED;
            }
            break;
          default:
            _btn_state = BTN_LONGCLICK;
            break;
          }
        }
        setFlag(ONECLICK_BIT, false);
      }
    }
  }
  // состояние кнопки изменилось с прошлого опроса
  else
  { // если задано подавление дребезга контактов
    if (_debounce_timeout > 0)
    { // если флаг подавления еще не поднят - поднять и больше ничего не делать
      if (!getFlag(DEBOUNCE_BIT))
      {
        btn_timer = thisMls;
        setFlag(DEBOUNCE_BIT, true);
        // и заодно сбросить переменную _btn_state, чтобы не выскакивали множественные события типа BTN_DOWN пока не истечет интервал антидребезга; исключение - состояние удержания кнопки в режиме непрерывного события
        if (!(_btn_state == BTN_LONGCLICK && _longclick_mode == LCM_CONTINUED))
        {
          _btn_state = isButtonClosed();
        }
      } // иначе, если поднят, и интервал вышел - установить состояние кнопки
      else if (thisMls - btn_timer >= _debounce_timeout)
      {
        btn_timer = thisMls;
        setBtnUpDown(isClosed, thisMls);
      }
    }
    else // если подавление вообще не задано, то сразу установить состояние кнопки
    {
      btn_timer = thisMls;
      setBtnUpDown(isClosed, thisMls);
    }
  }
  return (_btn_state);
}

uint8_t shButton::getButtonState()
{
  return (getButtonState(getContactsState()));
}

uint8_t shButton::getLastState()
{
  return (_btn_state);
}

bool shButton::isButtonClosed()
{
  return (getFlag(FLAG_BIT));
}

bool shButton::isSecondButtonPressed(shButton &_but, byte btn_state)
{
  bool result = false;
  if (getLastState() == btn_state && _but.isButtonClosed())
  {
    result = true;
    resetButtonState();
    _but.resetButtonState();
  }
  return (result);
}

void shButton::resetButtonState()
{
  setFlag(ONECLICK_BIT, false);
  setFlag(LONGCLICK_BIT, false);
#if defined(USE_BUTTON_FLAG)
  btn_flag = 0;
#endif
  // сброс _btn_state в зависимости от последнего состояния - либо нажата, либо отпущена
  _btn_state = isButtonClosed();
}

void shButton::setInputType(uint8_t inputtype)
{
  setFlag(INPUTTYPE_BIT, inputtype);
  switch (inputtype)
  {
  case PULL_UP:
    pinMode(_PIN, INPUT_PULLUP);
    break;
  default:
    pinMode(_PIN, INPUT);
    break;
  }
}

void shButton::setButtonType(uint8_t btntype)
{
  setFlag(BTNTYPE_BIT, btntype);
}

void shButton::setTimeoutOfDebounce(uint16_t new_timeout)
{
  _debounce_timeout = new_timeout;
}

void shButton::setTimeoutOfLongClick(uint16_t new_timeout)
{
  _longclick_timeout = new_timeout;
}

void shButton::setTimeoutOfDblClick(uint16_t new_timeout)
{
  _dblclck_timeout = new_timeout;
}

void shButton::setVirtualClickOn(bool virtualclick_on)
{
  setFlag(VIRTUALCLICK_BIT, virtualclick_on);
}

void shButton::setLongClickMode(uint8_t longclickmode)
{
  _longclick_mode = longclickmode;
  if (_longclick_mode == LCM_CLICKSERIES && _interval_of_serial == 0)
  {
    _interval_of_serial = INTERVAL_OF_SERIAL;
  }
}

void shButton::setIntervalOfSerial(uint16_t new_interval)
{
  _interval_of_serial = new_interval;
  // если установлено нулевое значение, то перевести режим на однократное событие
  if (_interval_of_serial == 0)
  {
    _longclick_mode = LCM_ONLYONCE;
  }
}

#if defined(USE_BUTTON_FLAG)
uint8_t shButton::getButtonFlag(bool _clear)
{
  uint8_t result = btn_flag;
  if (_clear)
  {
    btn_flag = 0;
  }
  return (result);
}

void shButton::setButtonFlag(uint8_t _flag)
{
  btn_flag = _flag;
}
#endif

// ==== private ======================================

bool shButton::getContactsState()
{
  bool val = false;
  if (!getFlag(VIRTUALBUTTON_BIT))
  {
    val = digitalRead(_PIN);
    if (getFlag(INPUTTYPE_BIT) == PULL_UP)
    {
      val = !val;
    }
    if (getFlag(BTNTYPE_BIT) == BTN_NC)
    {
      val = !val;
    }
  }
  return (val);
}

void shButton::setBtnUpDown(bool flag, uint32_t thisMls)
{
  setFlag(DEBOUNCE_BIT, false);
  setFlag(FLAG_BIT, flag);

  if (flag)
  {
    if (!getFlag(ONECLICK_BIT))
    { // если это первый клик, запустить таймер двойного клика и поднять флаг одиночного клика
      _btn_state = BTN_DOWN;
      setFlag(ONECLICK_BIT, true);
      dbl_timer = thisMls;
    }
    else if (thisMls - dbl_timer <= _dblclck_timeout)
    {
      _btn_state = BTN_DBLCLICK;
      setFlag(ONECLICK_BIT, false);
    }
  }
  else
  {
    _btn_state = BTN_UP;
  }
}

bool shButton::getFlag(uint8_t _bit)
{
  bool result = (_bit < 8) ? (((_flags) >> (_bit)) & 0x01) : false;
  return (result);
}

void shButton::setFlag(uint8_t _bit, bool x)
{
  if (_bit < 8)
  {
    (x) ? (_flags) |= (1UL << (_bit)) : (_flags) &= ~(1UL << (_bit));
  }
}

#endif