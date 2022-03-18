#pragma once

#include "../../common/EQ2Stream.h"
#include "../Packets/EncodedPackets.h"
#include "../Chat/ClientChat.h"
#include <map>

class ZoneServer;
class Spawn;
class PlayerController;

class Client : public EQ2Stream {
public:
	Client(unsigned int ip, unsigned short port);

	void Process();

	void SendLoginReply(uint8_t reply);

	EncodedPackets encoded_packets;
	void Disconnected() override;
	void ConnectionTimeout() override;

protected:
	void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) override;

private:

	uint32_t account_id;
	uint32_t character_id;
	uint16_t m_nextSpawnIndex;
	uint32_t m_nextSpawnID;

	std::weak_ptr<ZoneServer> m_zone;
	std::map<std::weak_ptr<Spawn>, uint16_t, std::owner_less<std::weak_ptr<Spawn>> > m_spawnIndexMap;
	std::map<uint16_t, std::weak_ptr<Spawn>> m_spawnIndexLookupMap;
	std::map<std::weak_ptr<Spawn>, uint32_t, std::owner_less<std::weak_ptr<Spawn>> > m_spawnIDMap;
	std::shared_ptr<PlayerController> m_controller;
	std::shared_ptr<class EncodedBuffer> defaultPosBuf;
	std::shared_ptr<class EncodedBuffer> defaultInfoBuf;
	std::shared_ptr<class EncodedBuffer> defaultVisBuf;

public:
	void SetAccountID(uint32_t val) { account_id = val; }
	uint32_t GetAccountID() { return account_id; }

	void SetCharacterID(uint32_t val) { character_id = val; }
	uint32_t GetCharacterID() { return character_id; }

	std::shared_ptr<ZoneServer> GetZone() { return m_zone.lock(); }
	void SetZone(std::weak_ptr<ZoneServer> zone) { m_zone = zone; }

	bool WasSentSpawn(const std::shared_ptr<Spawn>& spawn);
	uint16_t AddSpawnToIndexMap(const std::shared_ptr<Spawn>& spawn);
	void RemoveSpawnFromIndexMap(const std::shared_ptr<Spawn>& spawn);
	uint16_t GetIndexForSpawn(std::shared_ptr<Spawn> spawn);
	std::shared_ptr<Spawn> GetSpawnByIndex(uint16_t spawn_index);
	uint32_t AddSpawnToIDMap(const std::shared_ptr<Spawn>& spawn);
	uint32_t GetIDForSpawn(const std::shared_ptr<Spawn>& spawn);
	std::shared_ptr<Spawn> GetSpawnByID(uint32_t id);
	std::shared_ptr<PlayerController> GetController();

	void InitSpawnBuffers(uint32_t spawnIndex);

	ClientChat chat;
	bool bDevMode;
	bool bZoningDisconnect;
};