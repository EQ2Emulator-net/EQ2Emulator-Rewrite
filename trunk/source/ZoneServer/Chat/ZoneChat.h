#pragma once

#include <vector>
#include <memory>
#include "../../common/Packets/EQ2Packet.h"

class Entity;
class Client;
class ZoneServer;
class Spawn;

class ZoneChat {
public:
	ZoneChat(const std::map<uint32_t, std::weak_ptr<Client> >& clients, ZoneServer& zone);
	~ZoneChat() = default;

	void HandleSay(const char* msg, const std::shared_ptr<Spawn>& sender, uint8_t language = 0, const std::shared_ptr<Spawn>& receiver = std::shared_ptr<Spawn>());
	void HandleShout(const char* msg, const std::shared_ptr<Spawn>& sender, uint8_t language = 0);
	void HandleEmoteChat(const char* msg, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target = std::shared_ptr<Spawn>(), const std::shared_ptr<Spawn>& receiver = std::shared_ptr<Spawn>());
	void HandleOutOfCharacter(const char* msg, const std::shared_ptr<Spawn>& sender);
	void HandlePlayFlavor(struct PlayFlavorParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target = std::shared_ptr<Spawn>());
	//This function setup is a little weird but doing it this way for backwards compatability with old scripts/code
	void HandleCannedEmote(uint32_t anim, const char* msg, const std::shared_ptr<Spawn>& performer, const std::shared_ptr<Spawn>& targetSpawn = std::shared_ptr<Spawn>(), const std::shared_ptr<Spawn>& hideSpawn = std::shared_ptr<Spawn>(), uint8_t hideType = 0);

private:
	const std::map<uint32_t, std::weak_ptr<Client> >& zoneClients;
	ZoneServer& zone;
	float hearSpawnDistance;

	void HearChatClientsInRange(struct HearChatParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target = std::shared_ptr<Spawn>(), const std::shared_ptr<Spawn>& receiver = std::shared_ptr<Spawn>());
	void HearPlayFlavorClientsInRange(struct PlayFlavorParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target = std::shared_ptr<Spawn>());
};