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
        { // если период двойного клика закончился, проверить на виртуальный клик и обнулить счетчик кликов
          if (getFlag(VIRTUALCLICK_BIT) && _clckcount == 1)
          {
            _btnstate = BTN_ONECLICK;
          }
          _clckcount = 0;
          _longclickcount = 0;
        }
      }
      else if (millis() - btn_timer < _timeout)
      { // кнопка находится в нажатом состоянии, но время удержания еще не вышло
        _btnstate = BTN_PRESSED;
      }
      else
      { // если кнопка удерживается нажатой дольше времени удержания, то дальше возможны варианты
        switch (_longclickmode)
        {
        case LCM_ONLYONCE:
          if (_longclickcount == 0)
          {
            _longclickcount++;
            _btnstate = BTN_LONGCLICK;
          }
          else
          {
            _btnstate = BTN_PRESSED;
          }
          break;
        case LCM_CLICKSERIES:
          if (millis() - lclck_timer >= _longclicktimeout)
          {
            lclck_timer = thisMls;
            _longclickcount++;
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
        _clckcount = 0;
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
        deb_timer = thisMls;
        setFlag(DEBOUNCE_BIT, true);
      } // иначе, если поднят, и интервал вышел - установить состояние кнопки
      else if (millis() - deb_timer >= _debounce)
      {
        setBtnUpDown(isClosed, thisMls);
      }
    }
    else // если подавление вообще не задано, то сразу установить состояние кнопки
    {
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
  if (toChecked)
  {
    getButtonState();
  }
  // BTN_ONECLICK фактически тоже означает, что в данный момент кнопка не нажата (см. описание события)
  return _btnstate != BTN_RELEASED && _btnstate != BTN_UP && _btnstate != BTN_ONECLICK;
}

void shButton::resetButtonState()
{
  _clckcount = 0;
  _longclickcount = 0;
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
  bool val = digitalRead(_PIN);
  if (getFlag(INPUTTYPE_BIT) == PULL_UP)
  {
    val = !val;
  }
  if (getFlag(BTNTYPE_BIT) == BTN_NC)
  {
    val = !val;
  }
  return val;
}

void shButton::setBtnUpDown(bool flag, unsigned long thisMls)
{
  setFlag(DEBOUNCE_BIT, false);
  setFlag(FLAG_BIT, flag);

  if (flag)
  { // если кнопка только что нажата, то запустить таймер удержания
    btn_timer = thisMls;
    if (_clckcount == 0)
    { // если это первый клик, запустить таймер двойного клика и увеличить счетчик кликов
      _btnstate = BTN_DOWN;
      _clckcount++;
      dbl_timer = thisMls;
    }
    else if (millis() - dbl_timer <= _dblclck)
    {
      _btnstate = BTN_DBLCLICK;
      _clckcount = 0;
    }
  }
  else
  {
    _btnstate = BTN_UP;
  }
}

bool shButton::getFlag(byte _bit)
{
  bool result;
  (_bit < 8) ? result = (((_flags) >> (_bit)) & 0x01) : result = false;
  return (result);
}

void shButton::setFlag(byte _bit, bool x)
{
  if (_bit < 8)
  {
    (x) ? (_flags) |= (1UL << (_bit)) : (_flags) &= ~(1UL << (_bit));
  }
}
