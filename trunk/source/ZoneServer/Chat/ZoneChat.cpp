#include "stdafx.h"

#include "ZoneChat.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Entity.h"
#include "../../common/Rules.h"
#include "../ZoneServer/ZoneServer.h"
#include "../Packets/OP_EqCannedEmoteCmd_Packet.h"

extern RuleManager g_ruleManager;

ZoneChat::ZoneChat(const std::map<uint32_t, std::weak_ptr<Client> >& clients, ZoneServer& z) : zoneClients(clients), zone(z),
hearSpawnDistance(0.f) {
}

void ZoneChat::HandleSay(const char* msg, const std::shared_ptr<Spawn>& sender, uint8_t language, const std::shared_ptr<Spawn>& receiver) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = true;
	params.fromName = sender->GetName();
	params.chatFilterName = sender->IsNPC() ? "NPCSay" : "Say";
	params.language = language;
	params.toSpawnID = 0xFFFFFFFF;

	HearChatClientsInRange(params, sender, std::shared_ptr<Spawn>(), receiver);
}

void ZoneChat::HandleShout(const char* msg, const std::shared_ptr<Spawn>& sender, uint8_t language) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = true;
	params.fromName = sender->GetName();
	params.chatFilterName = "Shout";
	params.language = language;
	params.toSpawnID = 0xFFFFFFFF;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		if (uint32_t spawnID = client->GetIDForSpawn(sender)) {
			params.fromSpawnID = spawnID;
		}
		else {
			params.fromSpawnID = 0xFFFFFFFF;
		}

		client->chat.HearChat(params);
	}
}

//This function checks the distance of clients and whether they understand the language if set before sending a message
void ZoneChat::HearChatClientsInRange(HearChatParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target, const std::shared_ptr<Spawn>& receiver) {
	bool bSenderSent = false;
	bool bReceiverSent = false;

	LogWarn(LOG_CHAT, 0, "Check if the receiver understands a language in ZoneChat::HearChatClientsInRange to get rid of this spam!");
	params.bUnderstood = true;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		uint32_t spawnID;

		if (spawnID = client->GetIDForSpawn(sender)) {
			params.fromSpawnID = spawnID;
		}
		else {
			params.fromSpawnID = 0xFFFFFFFF;
		}

		if (target && (spawnID = client->GetIDForSpawn(target))) {
			params.toSpawnID = spawnID;
		}
		else {
			params.toSpawnID = 0xFFFFFFFF;
		}

		auto entity = client->GetController()->GetControlled();
		if (!entity) {
			continue;
		}

		if (sender == entity) {
			client->chat.HearChat(params);

			if (bReceiverSent) {
				break;
			}

			bSenderSent = true;
		}
		else if (!receiver || receiver == entity) {
			if (hearSpawnDistance == 0.f) {
				hearSpawnDistance = g_ruleManager.GetZoneRule(zone.GetID(), ERuleCategory::R_Zone, ERuleType::HearChatDistance)->GetFloat();
			}

			float distance = sender->GetDistance(entity);

			if (distance > hearSpawnDistance) {
				continue;
			}

			client->chat.HearChat(params);

			if (receiver) {
				if (bSenderSent) {
					break;
				}
				bReceiverSent = true;
			}
		}
	}
}

void ZoneChat::HandleEmoteChat(const char* msg, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target, const std::shared_ptr<Spawn>& receiver) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = false;
	params.fromName = sender->GetName();
	params.chatFilterName = "Emote";
	params.language = 0;
	params.toSpawnID = 0xFFFFFFFF;

	if (target) {
		params.toName = target->GetName();
	}

	HearChatClientsInRange(params, sender, target, receiver);
}

void ZoneChat::HandleOutOfCharacter(const char* msg, const std::shared_ptr<Spawn>& sender) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = false;
	params.fromName = sender->GetName();
	params.fromSpawnID = 0xFFFFFFFF;
	params.chatFilterName = "Out of Character";
	params.language = 0;
	params.toSpawnID = 0xFFFFFFFF;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		client->chat.HearChat(params);
	}
}

void ZoneChat::HandlePlayFlavor(PlayFlavorParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target) {
	HearPlayFlavorClientsInRange(params, sender, target);
}

void ZoneChat::HearPlayFlavorClientsInRange(PlayFlavorParams& params, const std::shared_ptr<Spawn>& sender, const std::shared_ptr<Spawn>& target) {
	LogWarn(LOG_CHAT, 0, "Check if the receiver understands a language in ZoneChat::HearPlayFlavorClientsInRange to get rid of this spam!");
	params.bUnderstood = true;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		if (uint32_t spawnID = client->GetIDForSpawn(sender)) {
			params.fromSpawnID = spawnID;
		}
		else {
			params.fromSpawnID = 0xFFFFFFFF;
		}

		auto entity = client->GetController()->GetControlled();
		if (!entity) {
			continue;
		}

		if (sender == entity) {
			client->chat.HearPlayFlavor(params);
		}
		else {
			if (hearSpawnDistance == 0.f) {
				hearSpawnDistance = g_ruleManager.GetZoneRule(zone.GetID(), ERuleCategory::R_Zone, ERuleType::HearChatDistance)->GetFloat();
			}

			float distance = sender->GetDistance(entity);

			if (distance > hearSpawnDistance) {
				continue;
			}

			client->chat.HearPlayFlavor(params);
		}
	}
}

void ZoneChat::HandleCannedEmote(uint32_t anim, const char* msg, const std::shared_ptr<Spawn>& performer, const std::shared_ptr<Spawn>& targetSpawn,
	const std::shared_ptr<Spawn>& hideSpawn, uint8_t hideType) 
{
	//hideType 1 = send to only that client
	//hideType 2 = send to every client but that one

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		auto player = client->GetController()->GetControlled();
		if (!player) {
			continue;
		}

		if (hearSpawnDistance == 0.f) {
			hearSpawnDistance = g_ruleManager.GetZoneRule(zone.GetID(), ERuleCategory::R_Zone, ERuleType::HearChatDistance)->GetFloat();
		}

		uint32_t performerID = client->GetIDForSpawn(performer);
		if (performerID == 0) {
			continue;
		}

		if (hideType == 1 && player != hideSpawn) {
			continue;
		}

		if (hideType == 2 && player == hideSpawn) {
			continue;
		}

		if (performer != player && performer->GetDistance(player) > hearSpawnDistance) {
			continue;
		}
		
		OP_EqCannedEmoteCmd_Packet p(client->GetVersion());
		p.animType = anim;
		p.emoteMsg = msg;
		p.performerID = performerID;
		
		if (targetSpawn) {
			uint32_t targetID = client->GetIDForSpawn(targetSpawn);
			if (targetID != 0) {
				p.targetID = targetID;
			}
		}

		client->QueuePacket(p);
	}
}