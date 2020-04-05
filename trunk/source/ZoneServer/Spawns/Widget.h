#pragma once

#include <cstdint>
#include <string>

enum class EWidgetType {
	EWIDGET_GENERIC,
	EWIDGET_DOOR
};

class Widget {
public:
	Widget();
	~Widget() = default;

private:
	EWidgetType m_widgetType;
	uint32_t m_widgetID;
	uint32_t m_actionSpawnID;
	uint32_t m_linkedSpawnID;
	uint32_t m_houseID;
	uint32_t m_openDuration;
	float m_widgetX;
	float m_widgetY;
	float m_widgetZ;
	float m_openX;
	float m_openY;
	float m_openZ;
	float m_closeX;
	float m_closeY;
	float m_closeZ;
	std::string m_openSoundFile;
	std::string m_closeSoundFile;
	bool m_includeHeading;
	bool m_includeLocation;
};