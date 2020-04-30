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
#include "../Packets/OP_UpdateTitleCmd_Packet.h"

Spawn::Spawn() : m_updateFlagsByte(0) {
	memset(&m_infoStruct, 0, sizeof(m_infoStruct));
	m_spawnID = GetNextID();
	movementTimestamp = Timer::GetServerTime();
	bShowName = true;
	bAttackable = false;
}

Spawn::Spawn(std::shared_ptr<Spawn> in) {
	memcpy(&m_infoStruct, in->GetInfoStruct(), sizeof(m_infoStruct));
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

	if (m_posStruct.collisionRadius == 0.0f)
		m_posStruct.collisionRadius = 1.0f;

	if (m_posStruct.sizeRatio == 0.0f)
		m_posStruct.sizeRatio = 1.0f;

	if (m_posStruct.size == 0.0f)
		m_posStruct.size = 1.0f;
	
	if (m_sizeOffset > 0.0f)
		m_posStruct.size = MakeRandom(m_posStruct.size, m_posStruct.size + m_sizeOffset);

	bAttackable = in->bAttackable;
	bShowName = in->bShowName;
}

Spawn::~Spawn() {

}

void Spawn::Process() {
	// m_controller->Process();

	if (m_updateFlagsByte > 0) {
		UpdateFlags spawnUpdateFlags = PopUpdateFlags();
		for (auto& itr : m_clients) {
			//We may need to modify these flags for this spawn
			UpdateFlags update = spawnUpdateFlags;
			std::shared_ptr<Client> client = itr.first.lock();

			if (!client) {
				continue;
			}

			uint16_t index = client->GetIndexForSpawn(shared_from_this());

			if (update.m_titleChanged) {
				OP_UpdateTitleCmd_Packet* p2 = new OP_UpdateTitleCmd_Packet(client->GetVersion());
				p2->spawn_id = index;

				if (m_titleStruct.m_updateFlags.nameChanged)
					p2->name = m_titleStruct.name;
				if (m_titleStruct.m_updateFlags.unknown1Changed)
					p2->unknown1 = m_titleStruct.unknown1;
				if (m_titleStruct.m_updateFlags.isPlayerChanged)
					p2->isPlayer = m_titleStruct.isPlayer;
				if (m_titleStruct.m_updateFlags.lastNameChanged)
					p2->last_name = m_titleStruct.last_name;
				if (m_titleStruct.m_updateFlags.suffixChanged)
					p2->suffix_title = m_titleStruct.suffix_title;
				if (m_titleStruct.m_updateFlags.prefixChanged)
					p2->prefix_title = m_titleStruct.prefix_title;
				if (m_titleStruct.m_updateFlags.pvpChanged)
					p2->pvp_title = m_titleStruct.pvp_title;
				if (m_titleStruct.m_updateFlags.guildChanged)
					p2->guild_title = m_titleStruct.guild;

				client->QueuePacket(p2);

				m_titleStruct.m_updateFlagsByte = 0;
			}

			// Don't send updates to yourself
			if (client->GetController()->GetControlled().get() == this) {
				update.m_posChanged = false;
			}

			OP_UpdateGhostCmdMsg_Packet* packet = nullptr;

			//Check vis first because we may not actually need to send an update for this client
			if (update.m_checkVisChange) {
				SpawnVisualizationStruct vis;
				vis.DetermineForClient(client, shared_from_this());
				uint32_t visCRC = vis.CalculateCRC();

				if (visCRC != itr.second) {
					//We have an update to send!
					itr.second = visCRC;
					packet = new OP_UpdateGhostCmdMsg_Packet(client->GetVersion());
					static_cast<SpawnVisualizationStruct&>(packet->vis.data) = vis;
					packet->vis.spawnIndex = index;
				}
				else if (!update.m_infoChanged && !update.m_posChanged) {
					continue;
				}
			}

			if (!packet)
				packet = new OP_UpdateGhostCmdMsg_Packet(client->GetVersion());

			packet->timestamp = Timer::GetServerTime();

			if (update.m_infoChanged)
				packet->InsertSpawnInfoData(*GetInfoStruct(), index);

			if (update.m_posChanged && client->GetController()->GetControlled().get() != this)
				packet->InsertSpawnPosData(*GetPosStruct(), index, true, movementTimestamp);

			packet->SetEncodedBuffers(client, index);
			client->QueuePacket(packet);
		}
	}
}

void Spawn::AddClient(std::weak_ptr<Client> client, uint32_t visCRC) {
	m_clients[client] = visCRC;
}

void Spawn::RemoveClient(std::weak_ptr<Client> client) {
	m_clients.erase(client);
}

Spawn::UpdateFlags Spawn::PopUpdateFlags() {
	UpdateFlags ret = m_updateFlags;
	m_updateFlagsByte = 0;
	return ret;
}

const SpawnPositionStruct* Spawn::GetPosStruct() const {
	return &m_posStruct;
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

	std::pair<int32_t, int32_t> oldCell = m_currentCellCoordinates;
	m_currentCellCoordinates = zone->GetCellCoordinatesForSpawn(shared_from_this());
	if (m_currentCellCoordinates != oldCell)
		zone->ChangeSpawnCell(shared_from_this(), oldCell);
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

void Spawn::SetAppearanceEquipmentItem(uint8_t slotID, const EQ2EquipmentItem& item, bool updateFlags) {
	SetInfo(&m_infoStruct.equipment_colors[slotID], item.color, false);
	SetInfo(&m_infoStruct.equipment_highlights[slotID], item.highlight, false);
	SetInfo(&m_infoStruct.equipment_types[slotID], item.type, updateFlags);
}

EConsiderDifficulty Spawn::GetConsiderDifficulty(uint8_t considererLevel, uint8_t targetLevel) {
	EConsiderDifficulty color;
	int16_t diff = targetLevel - considererLevel;
	if (considererLevel < 10)
		diff *= 3;
	else if (considererLevel <= 20)
		diff *= 2;
	if (diff >= 9)
		color = EConsiderDifficulty::RED;
	else if (diff >= 5)
		color = EConsiderDifficulty::ORANGE;
	else if (diff >= 1)
		color = EConsiderDifficulty::YELLOW;
	else if (diff == 0)
		color = EConsiderDifficulty::WHITE;
	else if (diff <= -11)
		color = EConsiderDifficulty::GRAY;
	else if (diff <= -6)
		color = EConsiderDifficulty::GREEN;
	else //if(diff < 0)
		color = EConsiderDifficulty::BLUE;
	return color;
}