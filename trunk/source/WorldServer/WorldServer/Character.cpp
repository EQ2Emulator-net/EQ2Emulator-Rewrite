#include "stdafx.h"

#include "Character.h"
#include "../ZoneTalk/ZoneStream.h"

void Character::SetZoneStream(const std::shared_ptr<ZoneStream>& stream, uint32_t session) {
	currentlyConnectedZone = stream;
	sessionID = session;
	if (!bOnline) {
		SetOnline();
	}
}

std::shared_ptr<ZoneStream> Character::GetZoneStream() {
	return currentlyConnectedZone.lock();
}

void Character::RemoveZoneStream(uint32_t session) {
	if (session == sessionID) {
		sessionID = 0;
		currentlyConnectedZone.reset();
	}

	if (!bPendingZoneConnection && bOnline) {
		SetOffline();
	}
}

void Character::SetOnline() {
	bOnline = true;
	//Do any kind of events we want to happen when a player comes online!
	LogDebug(LOG_WORLD, 0, "Setting character %s as online", basicInfo.characterName.c_str());
}

void Character::SetOffline() {
	bOnline = false;
	//Offline events here
	LogDebug(LOG_WORLD, 0, "Setting character %s as offline", basicInfo.characterName.c_str());
}

void Character::SetPendingZoneConnection() {
	bPendingZoneConnection = true;
	if (!bOnline) {
		SetOnline();
	}
}

bool Character::IsOnline() {
	return bOnline || bPendingZoneConnection;
}

void Character::PendingZoneConnectionConfirmed() {
	bPendingZoneConnection = false;
}

void Character::PendingZoneConnectionFailed() {
	bPendingZoneConnection = false;
	if (bOnline) {
		SetOffline();
	}
}