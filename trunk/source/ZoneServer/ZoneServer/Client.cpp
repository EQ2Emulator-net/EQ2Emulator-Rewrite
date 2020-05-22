#include "stdafx.h"

#include "Client.h"
#include "../Packets/OP_LoginByNumRequestMsg_Packet.h"
#include "../../common/EQ2Stream.h"

#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Entity.h"
#include "ZoneServer.h"
#include "ZoneOperator.h"
#include "../WorldTalk/WorldStream.h"
#include "../../common/Rules.h"
#include "../../common/Packets/EmuPackets/Emu_NotifyCharacterLinkdead_Packet.h"

extern ZoneOperator g_zoneOperator;
extern RuleManager g_ruleManager;

Client::Client(unsigned int ip, unsigned short port) : EQ2Stream(ip, port), chat(*this), m_nextSpawnIndex(1), m_nextSpawnID(1) {
	account_id = 0;
	character_id = 0;
	m_controller = std::make_shared<PlayerController>();
	bDevMode = false;
	bZoningDisconnect = false;
}

void Client::Process() {
	if (auto controlled = GetController()->GetControlled()) {
		const SpawnInfoStruct* info = controlled->GetInfoStruct();
		if (controlled->IsCamping()) {
			uint32_t now = Timer::GetServerTime();
			if (now >= info->activity_timer) {
				//This player has camped out, send a disconnect packet
				SendDisconnect(6);
			}
		}
	}

	while (EQ2Packet* p = PopPacket()) {
		if (NetDebugEnabled()) {
			LogError(LOG_PACKET, 0, "ZoneServer client packet dump");
			DumpBytes(p->buffer, p->Size);
		}
		p->HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
		delete p;
	}
}

void Client::ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) {
	//Choose the struct version depending on the size of the data
	uint32_t remaining_size = size - offset;
	uint32_t pver;
	if (remaining_size == 30) {
		pver = 283;
	}
	else if (remaining_size > 34) {
		pver = 1212;
	}
	else {
		pver = 284;
	}
	OP_LoginByNumRequestMsg_Packet p(pver);
	p.Read(data, offset, size);
	SetVersion(static_cast<uint32_t>(p.version));
	p.HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
}

void Client::SendLoginReply(uint8_t reply) {
	// Login error, 0 = accepted, 1 = invalid password, 2 = currently playing, 6 = bad version, every thing else = unknown reason
	LogDebug(LOG_CLIENT, 0, "SendLoginReply called with %u", reply);

	OP_LoginReplyMsg_Packet* r = new OP_LoginReplyMsg_Packet(GetVersion());
	r->Response = reply;
	/*if (reply == 0) {
		r->AccountID = AccountID;
		r->ResetAppearance = 0;
		r->DoNotForceSoga = 1;
		r->RaceUnknown = 63;
		r->Unknown11 = 7;
		r->SubscriptionLevel = 2;
		r->RaceFlag = GetServer()->GetAllowedRaces();	//0x001FFFFF;
		r->ClassFlag = GetServer()->GetAllowedClasses();	//0x07FFFFFE;

		// Unknown5 and Unknown7 set to DoV values
		r->Unknown5 = 1148;
		r->Unknown7 = 2145009599;

		r->Unknown10 = 1;
		r->NumClassItems = 0;
		r->UnknownArraySize = 0;
	}
	else {
		r->AccountID = 0xFFFFFFFF;
		r->ParentalControlTimer = 0xFFFFFFFF;
	}*/
	QueuePacket(r);
}

void Client::Disconnected() {
	std::shared_ptr<ZoneServer> zone = m_zone.lock();
	if (zone) {
		zone->RemoveClient(std::dynamic_pointer_cast<Client>(shared_from_this()));
	}
}

bool Client::WasSentSpawn(const std::shared_ptr<Spawn>& spawn) {
	return m_spawnIndexMap.count(spawn) != 0;
}

uint16_t Client::AddSpawnToIndexMap(const std::shared_ptr<Spawn>& spawn) {
	uint16_t index = 1;
	if (m_nextSpawnIndex == 0xFFFF) {
		//This client has exhausted the spawn indices available..find one that has opened back up
		if (!m_spawnIndexLookupMap.empty()) {
			//Should be impossible to be empty here but just incase
			uint16_t tmp = 1;
			uint16_t lowest = m_spawnIndexLookupMap.begin()->first;
			if (tmp < lowest) {
				index = tmp;
			}
			else {
				tmp = lowest;
				for (auto& itr : m_spawnIndexLookupMap) {
					if (tmp++ < itr.first) {
						index = tmp - 1;
						break;
					}
				}
			}
		}
	}
	else index = m_nextSpawnIndex++;

	

	m_spawnIndexMap[spawn] = index;
	m_spawnIndexLookupMap[index] = spawn;

	uint32_t spawnID = GetIDForSpawn(spawn);
	if (spawnID == 0)
		AddSpawnToIDMap(spawn);
	return index;
}

void Client::RemoveSpawnFromIndexMap(const std::shared_ptr<Spawn>& spawn) {
	std::map<std::weak_ptr<Spawn>, uint16_t>::iterator itr = m_spawnIndexMap.find(spawn);
	if (itr != m_spawnIndexMap.end()) {
		m_spawnIndexLookupMap.erase(itr->second);
		m_spawnIndexMap.erase(itr);
	}
	auto itr2 = m_spawnIDMap.find(spawn);
	if (itr2 != m_spawnIDMap.end()) {
		m_spawnIDMap.erase(itr2);
	}
}

uint16_t Client::GetIndexForSpawn(std::shared_ptr<Spawn> spawn) {
	uint16_t index = 0;

	std::map<std::weak_ptr<Spawn>, uint16_t>::iterator itr = m_spawnIndexMap.find(spawn);
	if (itr != m_spawnIndexMap.end())
		index = itr->second;

	return index;
}

std::shared_ptr<Spawn> Client::GetSpawnByIndex(uint16_t spawn_index) {
	auto itr = m_spawnIndexLookupMap.find(spawn_index);
	if (itr != m_spawnIndexLookupMap.end()) {
		return itr->second.lock();
	}

	return std::shared_ptr<Spawn>();
}

uint32_t Client::AddSpawnToIDMap(const std::shared_ptr<Spawn>& spawn) {
	uint32_t spawnID = m_nextSpawnID++;
	m_spawnIDMap[spawn] = spawnID;
	return spawnID;
}

uint32_t Client::GetIDForSpawn(const std::shared_ptr<Spawn>& spawn) {
	auto itr = m_spawnIDMap.find(spawn);
	if (itr != m_spawnIDMap.end()) {
		return itr->second;
	}

	return 0;
}

std::shared_ptr<Spawn> Client::GetSpawnByID(uint32_t id) {
	for (auto& itr : m_spawnIDMap) {
		if (itr.second == id) {
			return itr.first.lock();
		}
	}

	return std::shared_ptr<Spawn>();
}

std::shared_ptr<PlayerController> Client::GetController() {
	return m_controller;
}

void Client::ConnectionTimeout() {
	auto player = GetController()->GetControlled();
	if (!player || player->IsLinkdead()) {
		return;
	}

	CharacterSheet* sheet = GetController()->GetCharacterSheet();

	if (!sheet) {
		return;
	}

	static const uint32_t linkdeadTimeoutMS = g_ruleManager.GetGlobalRule(ERuleCategory::R_World, ERuleType::LinkDeadTimer)->GetUInt32();

	player->SetActivityTimer(Timer::GetServerTime() + linkdeadTimeoutMS);
	player->EnableEntityFlags(EntityFlagLinkdead);

	auto p = new Emu_NotifyCharacterLinkdead_Packet;
	p->characterID = sheet->characterID;
	g_zoneOperator.GetWorldStream()->QueuePacket(p);
}