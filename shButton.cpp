#include "shButton.h"
#include <Arduino.h>

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
  // BTN_ONECLICK фактически тоже означает, что в данный момент кнопка не нажата (см. описание события)
  return (_btn_state != BTN_RELEASED && _btn_state != BTN_UP && _btn_state != BTN_ONECLICK);
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

