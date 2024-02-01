## shButton - библиотека для работы с тактовыми кнопками. 

- [Возможности библиотеки:](#возможности-библиотеки)
- [История версий](#история-версий)
- [Работа с кнопкой](#работа-с-кнопкой)
- [Остальные методы класса `shButton`:](#остальные-методы-класса-shbutton)
- [Работа с пользовательским флагом кнопки](#работа-с-пользовательским-флагом-кнопки)


### Возможности библиотеки:
  - Работа с кнопками с нормально разомкнутыми и нормально замкнутыми контактами;
  - Работа с подключением `PULL_UP` и `PULL_DOWN`;
  - Работа с виртуальной кнопкой, т.е. кнопкой, не привязанной к конкретному пину, например - кнопки аналоговой, матричной клавиатуры или кнопки, подключенные через расширители портов;
  - Опрос кнопки как с программным подавлением дребезга контактов, так и без него; возможность настройки интервала антидребезга;
  - Отработка нажатия, отпускания кнопки, двойного клика; возможность настройки интервала двойного клика;
  - Возможность использования виртуального клика; в этом режиме событие клика (`BTN_ONECLICK`) генерируется по истечении интервала двойного клика, если не наступило событие двойного клика и в это же время кнопка не удерживается нажатой; таким образом появляется возможность организовать раздельную реакцию на однократный клик, двойной клик и удержание кнопки без написания дополнительного кода в скетче;
  - Отработка удержания кнопки; возможность настройки интервала удержания; 
  - Возможность настройки генерируемого при удержании кнопки свыше интервала удержания события (`BTN_LONGCLICK`) - непрерываная выдача события (т.е. фактически - состояния кнопки), однократная подача события или циклическая подача события через равные интервалы времени, пока кнопка нажата; возможность настройки этого интервала; 

### История версий
 Версия 1.8 - 12.02.2023
 * исправлена ошибка, которая могла приводить к множественному возникновению событий кнопки типа **BTN_DOWN** и т.д. в случае, если выполнение цикла `loop()` по какой-то причине затягивалось по времени, и в это время пользователь успевал изменить состояние кнопки; ошибка проявлялась как типичный дребезг контактов;
 * оптимизирован метод `isButtonClosed()`;

 Версия 1.7 - 01.02.2023
 + добавлена возможность установки произвольного пользовательского флага для кнопки; это может быть полезно, когда, например, функция, которой нужно считывать состояние кнопки, выполняется слишком редко и не может надежно получить состояние кнопки, опрашивая ее напрямую; см. пример в файле `UseButtonFlag.ino`;

 Версия 1.6 - 04.10.2022
 * метод `isButtonClosed()` - удален параметр `toChecked` как не имеющий практического смысла;
 * метод `setVirtualClickOn()` - установлен параметр по умолчанию `virtualclick_on = true`;
 * в метод `isSecondButtonPressed()` добавлен параметр `btn_state`, определяющий при возникновении какого события текущей кнопки будет проверяться состояние второй кнопки
 * упорядочена документация методов библиотеки;

 Версия 1.5 - 04.04.2022
 * упорядочены используемые типы данных;
 + добавлен метод `isSecondButtonPressed()`, позволяющий проверять, нажата ли вторая кнопка в момент клика текущей кнопки;
 * часть методов получила замену на аналогичные, но с более логичными именами:
    - метод `setLongClickTimeout()` помечен как устаревший и заменен методом `setIntervalOfSerial()`;
    - метод `setDebounce()` помечен как устаревший и заменен методом `setTimeoutOfDebounce()`;
    - метод `setTimeout()` помечен как устаревший и заменен методом `setTimeoutOfLongClick()`;
    - метод `setDblClickTimeout()` помечен как устаревший и заменен методом `setTimeoutOfDblClick()`;
 * обновлены примеры использования библиотеки;

 Версия 1.4 - 24.05.2021
 * слегка оптимизирован код, объем динамической памяти, занимаемой кнопкой, уменьшен c 30 до 20 байт;
 + добавлены примеры работы с виртуальной кнопкой на примере матричной клавиатуры и расширителя портов **MCP23017**;

 Версия 1.3 - 02.05.2021 
 + добавлен метод `getLastState()`, возвращающий состояние кнопки по результату последнего вызова метода `getButtonState()`;
 + добавлена возможность использования виртуальной кнопки, т.е. кнопки, не привязанной к конкретному пину; это дает возможность использовать, например, кнопки аналоговой или матричной клавиатуры; в случае виртуальной кнопки положение кнопки (нажата/не нажата) должно определяться внешним кодом и передаваться в виде аргумента в метод `getButtonState()`; для виртуальной кнопки доступны все возоможности обычной кнопки;

 Версия 1.2 - 19.11.2020
  * в методе `resetButtonState()` добавлен сброс счетчика _longclickcount и состояния кнопки `_btn_state`; состояние кнопки выставляется в зависимости от последнего состояния - либо нажата (`BTN_PRESSED`), либо отпущена (`BTN_RELEASED`);
  
 Версия 1.1 - 29.09.2020
  + в методе `isButtonClosed()` добавлен параметр toChecked; при значении **true** автоматически вызывает метод `getButtonState()` перед определением положения кнопки; при значении **false** (принято по умолчанию) определение положения кнопки выполняется по результату последнего опроса;
  + добавлен метод `resetButtonState()`, сбрасывающий счетчик кликов кнопки (_clckcount); метод позволяет исключить возникновение событий `BTN_ONECLICK` и `BTN_DBLCLICK`, если они по каким-то причинам в данный момент не желательны;
  + все булевы и часть байтовых полей упакованы в один байт, что позволило уменьшить размер занимаемой кнопкой памяти на 4 байта;

Версия 1.0 - 17.06.2020
 + первая версия;

### Работа с кнопкой
При работе с тактовыми кнопками нужно помнить о том, что пин, к которому подключена кнопка, должен иметь два устойчивых состояния. Иными словами, в зависимости от типа подключения кнопки пин должен быть притянут через резистор 10 кОм (или больше) к линии **VCC** (подключение `PULL_UP`) или **GND** (подключение `PULL_DOWN`). В случае подключени `PULL_UP` используется встроенная в микроконтроллер подтяжка пина к линии **VCC**, поэтому отдельный резистор не нужен, в случае `PULL_DOWN` подтяжка резистором к линии **GND** обязательна.

Работа с кнопкой начинается с создания объекта класса `shButton`:

`shButton btn(10, PULL_UP, BTN_NO);`

здесь:
- 10 - это пин, к которому подключена кнопка;
- `PULL_UP` - тип подключения - с подтяжкой к **VCC**, второй контакт кнопки при этом подключен к линии **GND**; т.е. подключение выглядит так: `pin -> кнопка -> GND`; при таком подключении подтягивающий резистор не нужен, т.к. в контроллере для пина автоматически будет задан режим `INPUT_PULLUP`;
- `BTN_NO` - тип кнопки, в данном случае с нормально разомкнутыми контактами;

Второй и третий параметры можно не указывать, тогда будут использованы параметры по умолчанию, т.е запись `shButton btn(10)`; эквивалентна записи `shButton btn(10, PULL_UP, BTN_NO)`;

Для виртуальной кнопки не указываются вообще никакие параметры:

`shButton btn;`

Одной из особенностей механических кнопок является дребезг контактов. Он обусловлен упругостью материалов, из которых изготовлены подвижные части контактов кнопки и выражается в многократном срабатывании при нажатии или отпускании кнопки. Иными словами вместо простого изменения состояния пина мы получаем несколько импульсов. Для борьбы с дребезгом используются как аппаратные решения (например RC-цепочки), так и программные. 

Класс `shButton` имеет возможность исключать влияние дребезга контактов программно. После изменения состояния пина (кнопка нажата или наоборот отпущена) результат выдается не сразу, а с некоторой задержкой. По умолчанию используется задержка 50 мс, но, если этой задержки недостаточно (например, слишком раздолбанная кнопка), то интервал можно изменить методом

`btn.setTimeoutOfDebounce(80);`

где 80 - новое значение интервала антидребезга в миллисекундах. Антидребезг можно отключить, если используется аппаратное подавление дребезга или, например, сенсорные кнопки, для этого достаточно указать нулевой интервал (`btn.setTimeoutOfDebounce(0)`).

Состояние кнопки определяется с помощью метода

`btn.getButtonState();`

Метод возвращает текущее состояние или событие кнопки. А именно:

**Состояния:**

- `BTN_RELEASED` (0) - кнопка отпущена;
- `BTN_PRESSED` (1) - кнопка нажата, но время удержания не вышло (или уже вышло, но событие BTN_LONGCLICK уже прошло);

**События**

- `BTN_UP` (2) - кнопка только что отпущена; событие возникает всегда в момент отпускания кнопки (с учетом антидребезга);
- `BTN_DOWN` (3) - кнопка только что нажата;
- `BTN_DBLCLICK` (4) - двойной клик; событие возникает, если повторный клик наступил в течение интервала двойного клика (по умолчанию 300 мс); таким образом при нажании на кнопку в любом случае происходит либо событие `BTN_DOWN`, либо `BTN_DBLCLICK`.
- `BTN_ONECLICK` (5) - виртуальный одиночный клик; следует через некоторый интервал после отпускания кнопки, если за это время не последовал двойной клик или длительное удержание кнопки нажатой; по умолчанию событие отключено, включается методом `btn.setVirtualClickOn(true)`; режим виртуального клика позволяет раздельно обрабатывать одиночный, двойной клик и удержание кнопки нажатой без написания дополнительного кода в скетче;

- `BTN_LONGCLICK` (6) - в зависимости от выбранного режима обработки удержиния кнопки нажатой может быть как состоянием - соответствует удержанию кнопки нажатой дольше интервала удержания (по умолчанию 500 мс), так и событием - однократным или следовать через определенные интервалы;

Второй вариант этого метода используется для работы с виртуальными кнопками, например, с аналоговой клавиатурой

`btn.getButtonState(isClosed);`

Метод работает точно так же, но сам не считывает текущее состояние кнопки. В данном случае состояние кнопки нужно определять самостоятельно и передавать в метод в виде аргумента: **true** - кнопка нажата, **false** - кнопка не нажата. См. примеры в папке **examples\VirtualButton**

Опрос кнопок должен производиться как можно чаще, чтобы обеспечить достаточную чувствительность обработки состояния кнопки. В состоянии покоя это не будет отнимать процессорное время, т.к. метод `getButtonState()` выполняет какие-либо действия только в случае, если состояние кнопки изменилось. Идеально вызывать его в каждом проходе `loop()`.

***Важно**: для надежной работы с кнопками метод `getButtonState()` нельзя вызывать более одного раза на один проход `loop()`. В противном случае вы рискуете пропускать события кнопки. Если результат вызова метода нужно использовать в нескольких местах, используйте метод*

`btn.getLastState();`

Режимы обработки удержания кнопки нажатой устанавливаются методом

`btn.setLongClickMode(mode);`

Где **mode** может принимать следующие значения:

- `LCM_CONTINUED` (0) - непрерывное событие `BTN_LONGCLICK`, генерируется постоянно, пока кнопка удерживается нажатой, если интервал удержания превышен; значение по умолчанию;
- `LCM_ONLYONCE` (1) - однократное событие `BTN_LONGCLICK`, генерируется только один раз по истечении интервала удержания кнопки, после чего генерируется состояние `BTN_PRESSED` до момента отпускания кнопки;
- `LCM_CLICKSERIES` (2) - по истечении интервала удержания кнопки событие `BTN_LONGCLICK` генерируется постоянно через равные интервалы времени, в промежутках генерируется состояние `BTN_PRESSED`; интервал по умолчанию - 200 мс;

Если вам нужно узнать текущее положение кнопки - нажата или нет, можно воспользоваться методом

`btn.isButtonClosed();`

который выдает **true** если кнопка нажата и **false** - если отпущена. Проверка выполняется по результату последнего опроса кнопки.

Если требуется зафиксировать одновременное нажатие двух кнопок, можно воспользоваться методом

`btn.isSecondButtonPressed(shButton &_but, byte btn_state = BTN_DOWN);`

который возвращает **true**, если в момент возникновения события `btn_state` текущей кнопки кнопка **_but**, переданная в метод в качестве аргумента, уже нажата; если метод возвратил **true**, состояние обеих кнопок будет сброшено, поэтому ни событие `BTN_ONECLICK`, ни `BTN_LONGCLICK` ни `BTN_DBLCLICK` для этих кнопок не наступят; метод работает по результатам последнего опроса кнопок; пример использования см. в файле **examples/PressTwoButton/PressTwoButton.ino**.

В некоторых случаях может понадобиться сброс состояния кнопки - если по каким-то причинам в данный момент не нужно, чтобы кнопка выдала события `BTN_ONECLICK`, `BTN_LONGCLICK` или `BTN_DBLCLICK`. Для этого используется метод

`btn.resetButtonState();`

который обнуляет внутренние счетчики кликов.

### Остальные методы класса `shButton`:

`void setInputType(uint8_t inputtype)`- установка типа подключения кнопки (`PULL_UP` - подтянута к **VCC**, `PULL_DOWN` - к **GND**)

`void setButtonType(uint8_t btntype)` - установка типа кнопки (`BTN_NO` - нормально разомкнутая (*normal open*), `BTN_NC` - нормально замкнутая(*normal close*))

`void setTimeoutOfDebounce(uint16_t new_timeout)` - установка интервала антидребезга (интервал по умолчанию 50 мс); для отключения антидребезга нужно задать 0 мс

`void setTimeoutOfLongClick(uint16_t new_timeout)` - установка интервала удержания кнопки (интервал по умолчанию 500 мс)

`void setTimeoutOfDblClick(uint16_t new_timeout)` - установка интервала двойного клика (интервал по умолчанию 300 мс)

`void setVirtualClickOn(bool virtualclick_on = true)` - включение (**true**) или отключение (**false**) режима "Виртуальный клик"; по умолчанию режим отключен

`void setLongClickMode(uint8_t longclickmode)` - установка режима обработки удержания кнопки нажатой (`LCM_CONTINUED`, `LCM_ONLYONCE` или `LCM_CLICKSERIES`)

`void setIntervalOfSerial(uint16_t new_interval)` - установка интервала выдачи события BTN_LONGCLICK в режиме `LCM_CLICKSERIES` (интервал по умолчанию 200 мс)

### Работа с пользовательским флагом кнопки

Иногда может возникнуть ситуация, когда функция, отслеживающая состояние кнопки, не может гарантированно получить данные о ее состоянии, т.к. запускается слишком редко. Например, если функция запускается через каждые 100 милисекунд, в перерыве между запусками функции кнопка может быть нажата и отпущена, соответственно, функция это нажатие "проморгает". Выходом из ситуации может быть организация отслеживания кнопки отдельной функцией, запускаемой непосредственно из loop(), и установки пользовательского флага при наступлении нужного события. А уже в целевой функции просто проверять наличие нужного флага у кнопки.

Т.к. эта опция может быть невостребованной, а место в памяти занимает, она скрыта под флагом условной компиляции, поэтому для ее использования нужно установить флаг `USE_BUTTON_FLAG` перед объявлением о включении файла **shButton.h** (см. файл `UseButtonFlag.ino` в папке примеров).

Пользовательские флаги объявляются самостоятельно на свое усмотрение, размерность - `uint8_t` (см. файл `UseButtonFlag.ino` в папке примеров).

Для работы с флагом кнопки используются два метода:

`void setButtonFlag(uint8_t _flag);` - установка пользовательского флага;

`uint8_t getButtonFlag(bool _clear = false);` - считывание пользовательского флага; если параметр `_clear` имеет значение **true**, после считывания флаг будет автоматически обнулен;

<hr>

Библиотека используется во всех моих проектах, где есть кнопки.

Если возникнут вопросы, пишите на valesh-soft@yandex.ru 
