#pragma once

#include <cstdint>
#include <string>

enum class ESignType {
	ESIGN_GENERIC,
	ESIGN_ZONE,
	ESIGN_PLAYER_MARKER
};

class Sign {
public:
	Sign();
	~Sign();

	void SetZoneID(uint32_t val) { m_zoneID = val; }
	void SetTitle(std::string val) { m_title = val; }
	void SetDescription(std::string val) { m_description = val; }
	void SetDistance(float val) { m_distance = val; }
	void SetZoneX(float val) { m_zoneX = val; }
	void SetZoneY(float val) { m_zoneY = val; }
	void SetZoneZ(float val) { m_zoneZ = val; }
	void SetZoneHeading(float val) { m_zoneHeading = val; }
	void SetSignType(ESignType val) { m_signType = val; }
	void SetSignDatabseID(uint32_t val) { m_signDatabaseID = val; }

	uint32_t GetZoneID() { return m_zoneID; }
	std::string GetTitle() { return m_title; }
	std::string GetDescription() { return m_description; }
	float GetDistance() { return m_distance; }
	float GetZoneX() { return m_zoneX; }
	float GetZoneY() { return m_zoneY; }
	float GetZoneZ() { return m_zoneZ; }
	float GetZoneHeading() { return m_zoneHeading; }
	ESignType GetSignType() { return m_signType; }
	uint32_t GetSignDatabaseID() { return m_signDatabaseID; }

private:
	uint32_t m_zoneID;
	std::string m_title;
	std::string m_description;
	float m_distance;
	float m_zoneX;
	float m_zoneY;
	float m_zoneZ;
	float m_zoneHeading;
	ESignType m_signType;
	uint32_t m_signDatabaseID;
};