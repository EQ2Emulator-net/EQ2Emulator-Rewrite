#include "stdafx.h"

#include "Spawn.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/BaseController.h"
#include "../Controllers/PlayerController.h"
#include "../../common/timer.h"
#include "Entity.h"

// Packets
#include "../Packets/OP_UpdateSpawnCmdMsg.h"

Spawn::Spawn() : m_updateFlagsByte(0) {
	memset(&m_infoStruct, 0, sizeof(m_infoStruct));
	memset(&m_visStruct, 0, sizeof(m_visStruct));
	m_spawnID = GetNextID();
	movementTimestamp = Timer::GetServerTime();
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