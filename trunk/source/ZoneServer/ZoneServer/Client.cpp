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
#include "../Packets/OP_CreateGhostCmd_Packet.h"
#include "../../common/EncodedBuffer.h"

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

	GetController()->Process();

	while (EQ2Packet* p = PopPacket()) {
		p->HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
		delete p;
	}
}

void Client::ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) {
	//Choose the struct version depending on the size of the data
	uint32_t pver = OP_LoginByNumRequestMsg_Packet::DetermineStructVersion(data, offset, size);
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
	if (reply == 0) {
		static const unsigned char unk7Bytes[] = { 0x7F, 0xFF, 0xFF, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE };
		r->bolUnknown4.resize(9);
		r->bolUnknown7.assign(reinterpret_cast<const char*>(unk7Bytes), 9);
		//r->AccountID = AccountID;
		//r->ResetAppearance = 0;
		//r->DoNotForceSoga = 1;
		//r->RaceUnknown = 63;
		//r->Unknown11 = 7;
		//r->SubscriptionLevel = 2;
		//r->RaceFlag = GetServer()->GetAllowedRaces();	//0x001FFFFF;
		//r->ClassFlag = GetServer()->GetAllowedClasses();	//0x07FFFFFE;

		//// Unknown5 and Unknown7 set to DoV values
		//r->Unknown5 = 1148;
		//r->Unknown7 = 2145009599;

		//r->Unknown10 = 1;
		//r->NumClassItems = 0;
		//r->UnknownArraySize = 0;
		r->Unknown14[12] = 1;
	}
	//else {
	//	r->AccountID = 0xFFFFFFFF;
	//	r->ParentalControlTimer = 0xFFFFFFFF;
	//}
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
	if (m_nextSpawnIndex == 0xFFFE) {
		//This client has exhausted the spawn indices available..find one that has opened back up
		if (!m_spawnIndexLookupMap.empty()) {
			//Should be impossible to be empty here but just incase
			uint16_t tmp = 1;
			auto itr = m_spawnIndexLookupMap.begin();
			uint16_t lowest = itr->first;
			if (tmp < lowest) {
				index = tmp;
			}
			else {
				tmp = lowest;
				for (++itr; itr != m_spawnIndexLookupMap.end(); ++itr) {
					if (++tmp < itr->first) {
						index = tmp;
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
	SendDisconnect(4);
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

void Client::InitSpawnBuffers(uint32_t spawnIndex) {
	if (!defaultPosBuf) {
		//We need to init our base buffers for this client/version
		defaultPosBuf = std::make_shared<EncodedBuffer>();
		defaultInfoBuf = std::make_shared<EncodedBuffer>();
		defaultVisBuf = std::make_shared<EncodedBuffer>();

		OP_CreateGhostCmd_Packet p(ClientVersion);
		p.SetXORDefaults();
		
		p.pos.SetEncodedBuffer(defaultPosBuf);
		p.info.SetEncodedBuffer(defaultInfoBuf);
		p.vis.SetEncodedBuffer(defaultVisBuf);

		//NOT proper but since we have been using this value sending it would flip some bits out
		p.info.visual_flag = 0;
		p.info.interaction_flag = 0;

		unsigned char* buf = nullptr;
		p.PreWrite();
		p.Write(buf);
		p.PostWrite();
	}

	//Check if these are fresh buffers, if so copy our default ones over
	bool bNewBuf;
	auto posBuf = encoded_packets.GetBuffer(EEncoded_UpdateSpawnPos, spawnIndex, &bNewBuf);
	if (bNewBuf) {
		auto infoBuf = encoded_packets.GetBuffer(EEncoded_UpdateSpawnInfo, spawnIndex);
		auto visBuf = encoded_packets.GetBuffer(EEncoded_UpdateSpawnVis, spawnIndex);

		*posBuf = *defaultPosBuf;
		*visBuf = *defaultVisBuf;
		*infoBuf = *defaultInfoBuf;
	}
}