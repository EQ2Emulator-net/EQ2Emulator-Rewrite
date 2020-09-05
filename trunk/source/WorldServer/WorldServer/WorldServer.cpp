#include "stdafx.h"

#include "WorldServer.h"
#include "Client.h"
#include "../../common/timer.h"
#include "../ZoneTalk/ZoneStream.h"
#include "../../common/Packets/EmuPackets/Emu_CancelClientTransfer_Packet.h"
#include "Character.h"

WorldServer::WorldServer() {
	ID = 0;
	Name = "Rewrite Test Server";
	Locked = false;
	CharacterSlotsPerAccount = 10;
	MaxAdvLevel = 50;
	MaxTSLevel = 50;
	AllowedRaces = 0x001FFFFF;
	AllowedClasses = 0x07FFFFFE;
	AutoAccountCreation = true;
}

std::shared_ptr<Stream> WorldServer::GetNewStream(unsigned int ip, unsigned short port) {
	return std::make_shared<Client>(ip, port);
}

bool WorldServer::Process() {
	uint32_t now = Timer::GetServerTime();

	//Check if a client failed to complete a transfer connection
	for (auto itr = pendingClientTransfers.begin(); itr != pendingClientTransfers.end();) {
		if (now >= itr->second.timeout) {
			//Let the zoneserver know to no longer expect this client

			if (std::shared_ptr<ZoneStream> zs = itr->second.stream.lock()) {
				auto p = new Emu_CancelClientTransfer_Packet;
				p->access_code = itr->first;
				zs->QueuePacket(p);
			}

			if (std::shared_ptr<Character> character = itr->second.character.lock()) {
				character->PendingZoneConnectionFailed();
			}

			itr = pendingClientTransfers.erase(itr);
			continue;
		}
		++itr;
	}

	return ProcessClientWrite() && ProcessClients();
}

bool WorldServer::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	for (auto& itr : Streams) {
		static_cast<Client*>(itr.second.get())->Write();
	}

	return true;
}

bool WorldServer::ProcessClients() {
	ReadLocker lock(streamLock);

	for (auto& itr : Streams) {
		static_cast<Client*>(itr.second.get())->Process();
	}

	return true;
}

std::shared_ptr<Client> WorldServer::GetClientByAccessCode(uint32_t code) {
	ReadLocker lock(streamLock);

	for (auto& itr : Streams) {
		if (static_cast<Client*>(itr.second.get())->pending_access_code == code) {
			return std::static_pointer_cast<Client>(itr.second);
		}
	}

	return std::shared_ptr<Client>();
}

void WorldServer::AddPendingClientTransfer(uint32_t access_code, const std::shared_ptr<Character>& character, const std::shared_ptr<ZoneStream>& stream) {
	PendingClientTransfer pct;
	pct.stream = stream;
	//60 second timeout
	pct.timeout = Timer::GetServerTime() + 60000;
	pct.character = character;
	pendingClientTransfers[access_code] = pct;
}

void WorldServer::ConfirmClientTransfer(uint32_t access_code, uint32_t sessionID, const std::string& zoneName) {
	auto itr = pendingClientTransfers.find(access_code);
	if (itr != pendingClientTransfers.end()) {
		const PendingClientTransfer& pct = itr->second;
		
		auto character = pct.character.lock();
		auto zs = pct.stream.lock();

		if (character) {
			character->SetZoneStream(zs, sessionID);
			//Handle events related to changing zones here
			character->basicInfo.currentZone = zoneName;
			character->PendingZoneConnectionConfirmed();
		}

		pendingClientTransfers.erase(itr);
	}
}