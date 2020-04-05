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

private:
	uint32_t m_zoneID;
	std::string m_title;
	std::string m_description;
	float m_distance;
	float m_zoneX;
	float m_zoneY;
	float m_zoneZ;
	float m_zoneHeading;
	bool m_includeHeading;
	bool m_includeLocation;
	ESignType m_signType;

};