#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

class ClientLogReply_Packet : public EQ2Packet {
public:
	ClientLogReply_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		name = "";
		log = "";
	}

	void HandlePacket(std::shared_ptr<Client> client) {
		client->SaveErrorsToDB(log, name);
	}

	std::string log;

protected:
	std::string name;

private:
	void RegisterElements() {
		if (GetVersion() > 283) {
			Register32String(log);
		}
		else {
			Register16String(log);
		}
	}

};