#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"
#include "OP_CampStartedMsg_Packet.h"
#include "../Spawns/Entity.h"
#include "../../common/Rules.h"
#include "../../common/thread.h"
#include "../ZoneServer/ZoneOperator.h"

extern RuleManager g_ruleManager;
extern ZoneOperator g_zoneOperator;

class OP_RequestCampMsg_Packet : public EQ2Packet {
public:
	OP_RequestCampMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
		bQuit = false;
		bCampDesktop = false;
		bCampCharSelect = false;
		unkBool = false;
		bLogin = false;
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		OP_CampStartedMsg_Packet p(client->GetVersion());

		if (!bCampDesktop && !bLogin && !bCampCharSelect) {
			p.bCampCharSelect = true;
			p.unkBool = true;
			p.charName = "characterselect";
			p.serverName = g_zoneOperator.GetWorldServerName();
		}
		else {
			p.bCampCharSelect = bCampCharSelect;
			p.unkBool = unkBool;
			p.charName = charName;
			p.serverName = serverName;
		}

		p.bCampDesktop = bCampDesktop;
		p.bLogin = bLogin;

		if (bQuit) {
			auto DelayedDisconnect = [](std::shared_ptr<Client> client) {
				//Wait until the client has received the camp message and then disconnect it
				SleepMS(1000);
				client->SendDisconnect(6);
			};

			ThreadManager::ThreadStart("RequestCampQuitHelper", std::bind(DelayedDisconnect, client)).detach();
		}
		else {
			//TODO: Check if admin to bypass camp timer
			static const uint8_t campSeconds = g_ruleManager.GetGlobalRule(ERuleCategory::R_World, ERuleType::PlayerCampTimer)->GetUInt8();
			p.numSeconds = campSeconds;
			if (std::shared_ptr<Entity> controlled = client->GetController()->GetControlled()) {
				controlled->SetActivityTimer(Timer::GetServerTime() + p.numSeconds * 1000, false);
				controlled->EnableEntityFlags(EntityFlagCamping);
			}
		}

		client->QueuePacket(p);
	}

	bool bQuit;
	bool bCampDesktop;
	bool bCampCharSelect;
	bool unkBool;
	std::string charName;
	std::string serverName;
	bool bLogin;

private:
	void RegisterElements() {
		//Classic just has 2 bytes, guessing it is just quit/camp desktop
		RegisterBool(bQuit);
		RegisterBool(bCampDesktop);
		if (GetVersion() <= 283) {
			return;
		}

		RegisterBool(bCampCharSelect);
		RegisterBool(unkBool);
		Register16String(charName);
		Register16String(serverName);

		if (GetVersion() >= 1153) {
			RegisterBool(bLogin);
		}
	}
};
