#include "shButton.h"
#include <Arduino.h>

shButton::shButton(byte pin, byte inputtype, byte btntype)
{
  _PIN = pin;
  setInputType(inputtype);
  setButtonType(btntype);
}

shButton::shButton()
{
  setFlag(VIRTUALBUTTON_BIT, true);
}

byte shButton::getButtonState(bool isClosed)
{
  unsigned long thisMls = millis();
  // состояние кнопки не изменилось с прошлого опроса
  if (isClosed == getFlag(FLAG_BIT))
  { // и не поднят флаг подавления дребезга
    if (!getFlag(DEBOUNCE_BIT))
    {
      if (!isClosed)
      { // кнопка находится в отжатом состоянии
        _btnstate = BTN_RELEASED;
        if (millis() - dbl_timer > _dblclck)
        { // если период двойного клика закончился, проверить на виртуальный клик и сбросить флаг одиночного клика
          if (getFlag(VIRTUALCLICK_BIT) && getFlag(ONECLICK_BIT))
          {
            _btnstate = BTN_ONECLICK;
          }
          setFlag(ONECLICK_BIT, false);
          setFlag(LONGCLICK_BIT, false);
        }
      }
      else if (millis() - btn_timer < _timeout)
      { // кнопка находится в нажатом состоянии, но время удержания еще не вышло
        _btnstate = BTN_PRESSED;
      }
      else // если кнопка удерживается нажатой дольше времени удержания, то дальше возможны варианты
      {    // если события удержания еще не было, то выдать его
        if (!getFlag(LONGCLICK_BIT))
        {
          if (_longclickmode == LCM_CLICKSERIES)
          {
            btn_timer = thisMls;
          }
          setFlag(LONGCLICK_BIT, true);
          _btnstate = BTN_LONGCLICK;
        }
        else // если это уже не первое событие удержания, то дальше согласно выбранного режима
        {
          switch (_longclickmode)
          {
          case LCM_ONLYONCE:
            _btnstate = BTN_PRESSED;
            break;
          case LCM_CLICKSERIES:
            if (millis() - btn_timer >= _longclicktimeout)
            {
              btn_timer = thisMls;
              _btnstate = BTN_LONGCLICK;
            }
            else
            {
              _btnstate = BTN_PRESSED;
            }
            break;
          default:
            _btnstate = BTN_LONGCLICK;
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
    if (_debounce > 0)
    { // если флаг подавления еще не поднят - поднять и больше ничего не делать
      if (!getFlag(DEBOUNCE_BIT))
      {
        btn_timer = thisMls; 
        setFlag(DEBOUNCE_BIT, true);
      } // иначе, если поднят, и интервал вышел - установить состояние кнопки
      else if (millis() - btn_timer >= _debounce)
      {
        setBtnUpDown(isClosed, thisMls);
      }
    }
    else // если подавление вообще не задано, то сразу установить состояние кнопки
    {
      btn_timer = thisMls;
      setBtnUpDown(isClosed, thisMls);
    }
  }
  return _btnstate;
}

byte shButton::getButtonState()
{
  return (getButtonState(getButtonFlag()));
}

byte shButton::getLastState()
{
  return _btnstate;
}

bool shButton::isButtonClosed(bool toChecked)
{
  if ((toChecked) && (!getFlag(VIRTUALBUTTON_BIT)))
  {
    getButtonState();
  }
  // BTN_ONECLICK фактически тоже означает, что в данный момент кнопка не нажата (см. описание события)
  return _btnstate != BTN_RELEASED && _btnstate != BTN_UP && _btnstate != BTN_ONECLICK;
}

void shButton::resetButtonState()
{
  setFlag(ONECLICK_BIT, false);
  setFlag(LONGCLICK_BIT, false);
  // сброс _btnstate в зависимости от последнего состояния - либо нажата, либо отпущена
  _btnstate = isButtonClosed();
}

void shButton::setInputType(byte inputtype)
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

void shButton::setButtonType(byte btntype)
{
  setFlag(BTNTYPE_BIT, btntype);
}

void shButton::setDebounce(word debounce)
{
  _debounce = debounce;
}

void shButton::setTimeout(word new_timeout)
{
  _timeout = new_timeout;
}

void shButton::setDblClickTimeout(word new_timeout)
{
  _dblclck = new_timeout;
}

void shButton::setVirtualClickOn(bool virtualclick)
{
  setFlag(VIRTUALCLICK_BIT, virtualclick);
}

void shButton::setLongClickMode(byte longclickmode)
{
  _longclickmode = longclickmode;
  if (_longclickmode == LCM_CLICKSERIES && _longclicktimeout == 0)
  {
    _longclicktimeout = LONGCLICKTIMEOUT;
  }
}

void shButton::setLongClickTimeout(word longclicktimeout)
{
  _longclicktimeout = longclicktimeout;
  // если установлено нулевое значение, то перевести режим на однократное событие
  if (_longclicktimeout == 0)
  {
    _longclickmode = LCM_ONLYONCE;
  }
}

bool shButton::getButtonFlag()
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
  return val;
}

void shButton::setBtnUpDown(bool flag, unsigned long thisMls)
{
  setFlag(DEBOUNCE_BIT, false);
  setFlag(FLAG_BIT, flag);

  if (flag)
  {
    if (!getFlag(ONECLICK_BIT))
    { // если это первый клик, запустить таймер двойного клика и поднять флаг одиночного клика
      _btnstate = BTN_DOWN;
      setFlag(ONECLICK_BIT, true);
      dbl_timer = thisMls;
    }
    else if (millis() - dbl_timer <= _dblclck)
    {
      _btnstate = BTN_DBLCLICK;
      setFlag(ONECLICK_BIT, false);
    }
  }
  else
  {
    _btnstate = BTN_UP;
  }
}

bool shButton::getFlag(byte _bit)
{
  bool result = (_bit < 8) ? (((_flags) >> (_bit)) & 0x01) : false;
  return (result);
}

void shButton::setFlag(byte _bit, bool x)
{
  if (_bit < 8)
  {
    (x) ? (_flags) |= (1UL << (_bit)) : (_flags) &= ~(1UL << (_bit));
  }
}
