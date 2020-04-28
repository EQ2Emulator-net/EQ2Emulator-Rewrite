#include "stdafx.h"

#include "Spawn.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/BaseController.h"
#include "../Controllers/PlayerController.h"
#include "../../common/timer.h"
#include "Entity.h"
#include "../ZoneServer/ZoneServer.h"

// Packets
#include "../Packets/OP_UpdateSpawnCmdMsg.h"

Spawn::Spawn() : m_updateFlagsByte(0) {
	memset(&m_infoStruct, 0, sizeof(m_infoStruct));
	memset(&m_visStruct, 0, sizeof(m_visStruct));
	m_spawnID = GetNextID();
	movementTimestamp = Timer::GetServerTime();
}

Spawn::Spawn(std::shared_ptr<Spawn> in) {
	memcpy(&m_infoStruct, in->GetInfoStruct(), sizeof(m_infoStruct));
	memcpy(&m_visStruct, in->GetVisStruct(), sizeof(m_visStruct));
	memcpy(&m_posStruct, in->GetPosStruct(), sizeof(m_infoStruct));
	m_spawnID = GetNextID();
	movementTimestamp = Timer::GetServerTime();
	m_titleStruct.name = in->GetTitleStruct()->name;
	m_titleStruct.last_name = in->GetTitleStruct()->last_name;
	m_titleStruct.guild = in->GetTitleStruct()->guild;
	m_titleStruct.prefix_title = in->GetTitleStruct()->prefix_title;
	m_titleStruct.suffix_title = in->GetTitleStruct()->suffix_title;
	m_titleStruct.pvp_title = in->GetTitleStruct()->pvp_title;
	m_titleStruct.isPlayer = in->GetTitleStruct()->isPlayer;
	m_titleStruct.unknown1 = in->GetTitleStruct()->unknown1;
	m_updateFlagsByte = 0;
	if (in->GetSignData() != nullptr)
		signData = std::make_unique<Sign>(*in->GetSignData());
	if (in->GetWidgetData() != nullptr)
		widgetData = std::make_unique<Widget>(*in->GetWidgetData());
	m_spawnID = GetNextID();
	m_spawnDatabaseID = in->GetDatabaseID();
	m_sizeOffset = in->GetSizeOffset();
	m_primaryCommandListID = in->GetPrimaryCommandListID();
	m_secondaryCommandListID = in->GetSecondaryCommandListID();
	m_factionID = in->GetFactionID();
	m_hp = in->GetHP();
	m_power = in->GetPower();
	m_savagery = in->GetSavagery();
	m_dissonance = in->GetDissonance();
	m_merchantID = in->GetMerchantID();
	m_merchantType = in->GetMerchantType();
	m_spawnLocationID = 0;
	m_spawnEntryID = 0;
	m_respawnTime = 0;
	m_expireTime = 0;
	m_spawnLocationPlacementID = 0;
	m_origX = 0.0f;
	m_origY = 0.0f;
	m_origZ = 0.0f;
	m_origHeading = 0.0f;
	m_origPitch = 0.0f;
	m_origRoll = 0.0f;

	if (m_posStruct.collisionRadius == 0)
		m_posStruct.collisionRadius = 1.0f;

	if (m_posStruct.sizeRatio == 0)
		m_posStruct.sizeRatio = 1.0f;

	if (m_posStruct.sizeMultiplierRatio == 0)
		m_posStruct.sizeMultiplierRatio = 1.0f;

	if (m_posStruct.size == 0)
		m_posStruct.size = 1.0f;
	
	if (m_sizeOffset > 0)
		m_posStruct.size = MakeRandom(m_posStruct.size, m_posStruct.size + m_sizeOffset);
}

Spawn::~Spawn() {

}

void Spawn::Process() {
	// m_controller->Process();

	if (m_updateFlagsByte > 0) {
		UpdateFlags update = PopUpdateFlags();
		for (std::weak_ptr<Client> c : m_clients) {
			std::shared_ptr<Client> client = c.lock();
			if (client) {
				// Don't send updates to yourself
				if (client->GetController()->GetControlled().get() == this && !update.m_infoChanged && !update.m_visChanged)
					continue;

				OP_UpdateGhostCmdMsg_Packet* packet = new OP_UpdateGhostCmdMsg_Packet(client->GetVersion());
				uint16_t index = client->GetIndexForSpawn(shared_from_this());
				packet->timestamp = Timer::GetServerTime();

				if (update.m_infoChanged)
					packet->InsertSpawnInfoData(*GetInfoStruct(), index);

				if (update.m_posChanged && client->GetController()->GetControlled().get() != this)
					packet->InsertSpawnPosData(*GetPosStruct(), index, true, movementTimestamp);

				if (update.m_visChanged)
					packet->InsertSpawnVisData(*GetVisStruct(), index);

				packet->SetEncodedBuffers(client, index);
				client->QueuePacket(packet);
			}
		}
	}
}

void Spawn::AddClient(std::weak_ptr<Client> client) {
	m_clients.push_back(client);
}

Spawn::UpdateFlags Spawn::PopUpdateFlags() {
	UpdateFlags ret = m_updateFlags;
	m_updateFlagsByte = 0;
	return ret;
}

const SpawnPositionStruct* Spawn::GetPosStruct() const {
	return &m_posStruct;
}

const SpawnVisualizationStruct* Spawn::GetVisStruct() const {
	return &m_visStruct;
}

const SpawnInfoStruct* Spawn::GetInfoStruct() const {
	return &m_infoStruct;
}

const SpawnTitleStruct* Spawn::GetTitleStruct() const {
	return &m_titleStruct;
}

uint32_t Spawn::GetNextID() {
	static std::atomic<uint32_t> g_spawnID(1);
	uint32_t ret;
	//There was a comment in old code about the ID ending in 255 crashing the client, not sure if it's true but doing it anyway
	while (ret = g_spawnID.fetch_add(1), (ret & 0xFF) == 0xFF);
	return ret;
}

void Spawn::UpdateCellCoordinates() {
	std::shared_ptr<ZoneServer> zone = m_zone.lock();
	if (!zone)
		return;

	std::pair<int32_t, int32_t> newCoords = zone->GettCellCoordinatesForSpawn(shared_from_this());
	if (newCoords != m_currentCellCoordinates) {
		m_currentCellCoordinates = newCoords;
	}
}

float Spawn::GetDistance(float x1, float y1, float z1, float x2, float y2, float z2, bool ignore_y) {
	x1 -= x2;
	z1 -= z2;

	x1 *= x1;
	z1 *= z1;

	if (ignore_y) {
		y1 = 0.f;
	}
	else {
		y1 -= y2;
		y1 *= y1;
	}

	return std::sqrt(x1 + y1 + z1);
}

float Spawn::GetDistance(float x, float y, float z, bool ignore_y) {
	return GetDistance(x, y, z, GetX(), GetY(), GetZ(), ignore_y);
}

float Spawn::GetDistance(const std::shared_ptr<Spawn>& spawn, bool ignore_y) {
	return GetDistance(spawn->GetX(), spawn->GetY(), spawn->GetZ(), ignore_y);
}