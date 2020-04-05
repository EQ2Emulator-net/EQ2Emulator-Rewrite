#include "stdafx.h"

#include "Widget.h"

Widget::Widget():
m_widgetID(0),
m_actionSpawnID(0),
m_linkedSpawnID(0),
m_houseID(0),
m_openDuration(0),
m_widgetX(0.f),
m_widgetY(0.f),
m_widgetZ(0.f),
m_openX(0.f),
m_openY(0.f),
m_openZ(0.f),
m_closeX(0.f),
m_closeY(0.f),
m_closeZ(0.f),
m_includeHeading(true),
m_includeLocation(true),
m_widgetType(EWidgetType::EWIDGET_GENERIC)
{

}