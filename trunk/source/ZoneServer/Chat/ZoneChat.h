#pragma once

#include <vector>
#include <memory>

class Entity;
class Client;
class ZoneServer;
class Spawn;

class ZoneChat {
public:
	ZoneChat(const std::map<uint32_t, std::weak_ptr<Client> >& clients, ZoneServer& zone);
	~ZoneChat() = default;

	void HandleSay(const char* msg, const std::shared_ptr<Entity>& sender, uint8_t language = 0, const std::shared_ptr<Spawn>& receiver = std::shared_ptr<Spawn>());
	void HandleShout(const char* msg, const std::shared_ptr<Entity>& sender, uint8_t language = 0);
	void HandleEmoteChat(const char* msg, const std::shared_ptr<Entity>& sender);
	void HandleOutOfCharacter(const char* msg, const std::shared_ptr<Entity>& sender);

private:
	const std::map<uint32_t, std::weak_ptr<Client> >& zoneClients;
	ZoneServer& zone;
	float hearSpawnDistance;

	void HearChatClientsInRange(struct HearChatParams& params, const std::shared_ptr<Entity>& sender, const std::shared_ptr<Spawn>& receiver = std::shared_ptr<Spawn>());
};