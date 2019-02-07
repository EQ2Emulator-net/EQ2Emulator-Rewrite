#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

class ClientLogReply_Packet : public EQ2Packet {
public:
	ClientLogReply_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();

		name = "";
		log = "";
	}

	void HandlePacket(Client* client) {
		client->SaveErrorsToDB(log, name);
	}

	std::string log;

protected:
	std::string name;

private:
	void RegisterElements() {
		Register32String(log);
	}

};