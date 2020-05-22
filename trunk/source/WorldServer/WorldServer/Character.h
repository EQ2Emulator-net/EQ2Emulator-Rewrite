#pragma once

#include "../../common/CharacterBasicInfo.h"
#include <memory>

class ZoneStream;

class Character : std::enable_shared_from_this<Character> {
public:
	Character() : sessionID(0), bPendingZoneConnection(false), bOnline(false), bLinkdead(false) {}
	~Character() = default;

	CharacterBasicInfo basicInfo;

	void SetZoneStream(const std::shared_ptr<ZoneStream>& stream, uint32_t sessionID);
	std::shared_ptr<ZoneStream> GetZoneStream();
	void RemoveZoneStream(uint32_t sessionID);

	//Returns true if there is already a pending zone connection
	void SetPendingZoneConnection();
	bool IsOnline();
	void PendingZoneConnectionConfirmed();
	void PendingZoneConnectionFailed();

	uint32_t GetSessionID() { return sessionID; }

	void SetLinkdead() { bLinkdead = true; }
	bool IsLinkdead() { return bLinkdead; }

	void SetOffline();

private:
	void SetOnline();

	uint32_t sessionID;
	std::weak_ptr<ZoneStream> currentlyConnectedZone;
	bool bOnline;
	bool bPendingZoneConnection;
	bool bLinkdead;
};