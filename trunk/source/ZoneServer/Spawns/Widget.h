#pragma once

#include <cstdint>
#include <string>

enum class EWidgetType {
	EWIDGET_GENERIC,
	EWIDGET_DOOR,
	EWIDGET_LIFT
};

class Widget {
public:
	Widget();
	~Widget() = default;

	void SetWidgetType(EWidgetType val) { m_widgetType = val; }
	void SetWidgetID(uint32_t val) { m_widgetID = val; }
	void SetActionID(uint32_t val) { m_actionSpawnID = val; }
	void SetLinkedSpawnID(uint32_t val) { m_linkedSpawnID = val; }
	void SetHouseID(uint32_t val) { m_houseID = val; }
	void SetOpenDuration(uint32_t val) { m_openDuration = val; }
	void SetWidgetX(float val) { m_widgetX = val; }
	void SetWidgetY(float val) { m_widgetY = val; }
	void SetWidgetZ(float val) { m_widgetZ = val; }
	void SetOpenX(float val) { m_openX = val; }
	void SetOpenY(float val) { m_openY = val; }
	void SetOpenZ(float val) { m_openZ = val; }
	void SetCloseX(float val) { m_closeX = val; }
	void SetCloseY(float val) { m_closeY = val; }
	void SetCloseZ(float val) { m_closeZ = val; }
	void SetOpenHeading(float val) { m_openHeading = val; }
	void SetCloseHeading(float val) { m_closeHeading = val; }
	void SetOpenSoundFile(std::string val) { m_openSoundFile = val; }
	void SetCloseSoundFile(std::string val) { m_closeSoundFile = val; }
	void SetIncludeHeading(bool val) { m_includeHeading = val; }
	void SetIncludeLocation(bool val) { m_includeLocation = val; }
	void SetWidgetDatabaseID(uint32_t val) { m_widgetDatabaseID = val; }

	EWidgetType GetWidgetType() { return m_widgetType; }
	uint32_t GetWidgetID() { return m_widgetID; }
	uint32_t GetActionSpawnID() { return m_actionSpawnID; }
	uint32_t GetLinkedSpawnID() { return m_linkedSpawnID; }
	uint32_t GetHouseID() { return m_houseID; }
	uint32_t GetOpenDuration() { return m_openDuration; }
	float GetWidgetX() { return m_widgetX; }
	float GetWidgetY() { return m_widgetY; }
	float GetWidgetZ() { return m_widgetZ; }
	float GetOpenX() { return m_openX; }
	float GetOpenY() { return m_openY; }
	float GetOpenZ() { return m_openZ; }
	float GetCloseX() { return m_closeX; }
	float GetCloseY() { return m_closeY; }
	float GetCloseZ() { return m_closeZ; }
	float GetOpenHeading() { return m_openHeading; }
	float GetCloseHeading() { return m_closeHeading; }
	std::string GetOpenSoundFile() { return m_openSoundFile; }
	std::string GetCloseSoundFile() { return m_closeSoundFile; }
	bool GetIncludeHeading() { return m_includeHeading; }
	bool GetIncludeLocation() { return m_includeLocation; }
	uint32_t GetWidgetDatabaseID() { return m_widgetDatabaseID; }

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
	float m_openHeading;
	float m_closeHeading;
	std::string m_openSoundFile;
	std::string m_closeSoundFile;
	bool m_includeHeading;
	bool m_includeLocation;
	uint32_t m_widgetDatabaseID;
};