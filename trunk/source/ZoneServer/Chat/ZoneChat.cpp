#include "stdafx.h"

#include "ZoneChat.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Entity.h"
#include "../../common/Rules.h"
#include "../ZoneServer/ZoneServer.h"

extern RuleManager g_ruleManager;

ZoneChat::ZoneChat(const std::map<uint32_t, std::weak_ptr<Client> >& clients, ZoneServer& z) : zoneClients(clients), zone(z),
hearSpawnDistance(0.f) {
}

void ZoneChat::HandleSay(const char* msg, const std::shared_ptr<Entity>& sender, uint8_t language, const std::shared_ptr<Spawn>& receiver) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = true;
	params.fromName = sender->GetName();
	params.fromSpawnID = sender->GetID();
	params.chatFilterName = "Say";
	params.language = language;
	params.toSpawnID = 0xFFFFFFFF;

	HearChatClientsInRange(params, sender, receiver);
}

void ZoneChat::HandleShout(const char* msg, const std::shared_ptr<Entity>& sender, uint8_t language) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = true;
	params.fromName = sender->GetName();
	params.fromSpawnID = sender->GetID();
	params.chatFilterName = "Shout";
	params.language = language;
	params.toSpawnID = 0xFFFFFFFF;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
		}

		client->chat.HearChat(params);
	}
}

//This function checks the distance of clients and whether they understand the language if set before sending a message
void ZoneChat::HearChatClientsInRange(HearChatParams& params, const std::shared_ptr<Entity>& sender, const std::shared_ptr<Spawn>& receiver) {
	bool bSenderSent = false;
	bool bReceiverSent = false;

	LogWarn(LOG_CHAT, 0, "Check if the receiver understands a language in ZoneChat::HearChatClientsInRange to get rid of this spam!");
	params.bUnderstood = true;

	for (auto& itr : zoneClients) {
		auto client = itr.second.lock();

		if (!client) {
			continue;
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

void ZoneChat::HandleEmoteChat(const char* msg, const std::shared_ptr<Entity>& sender) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = false;
	params.fromName = sender->GetName();
	params.fromSpawnID = sender->GetID();
	params.chatFilterName = "Emote";
	params.language = 0;
	params.toSpawnID = 0xFFFFFFFF;

	HearChatClientsInRange(params, sender);
}

void ZoneChat::HandleOutOfCharacter(const char* msg, const std::shared_ptr<Entity>& sender) {
	HearChatParams params;
	params.message = msg;
	params.bFromNPC = sender->IsNPC();
	params.bShowBubble = false;
	params.fromName = sender->GetName();
	params.fromSpawnID = sender->GetID();
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