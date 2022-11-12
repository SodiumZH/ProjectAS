# NaWidgets实用控件包开发目标

NaWidgets主要基于Slate开发可**直接**用于游戏UI设计的实用控件集。同时实现UMG封装，便于UI设计师使用而不需接触底层代码。

开发目标 220302：

##### 方形槽集合

如传统RPG游戏中的道具栏、技能栏等，主要通过SWrapBox实现数组形式的可交互方形槽位的集合，同时提供对外接口与道具栏、技能信息等直接交互。

SNaBoxSlot：单个的方形槽，提供选择、鼠标悬浮、拖动功能。

TNaBoxSlotList<class SlotType = SNaBoxSlot>：方形槽集合的模板类，通过SWrapBox实现。

##### 可调大小边框与窗口栏

如游戏中的血条、经验条、窗口等，共同特点为可调整大小，且具有在调整大小过程中宽度不变的边框。

SNaHorizontalGauge：横版血条，提供计量值（反映实际血量等），可调长度而两端不变。

SNaVerticalGauge：竖版血条，功能同上。

SNaResizableFrame：可调大小的带边框窗口，在大小调整时边框宽度不变。

SNaDialogBox：在ResizableFrame的基础上，上方增加一个标题块，支持拖动、最小化、关闭功能。标题块支持拖动。中央放一个插槽，可随意加入子控件。

SNaVerticalDialogBox：类似SNaDialogBox，但标题块位于左侧。

