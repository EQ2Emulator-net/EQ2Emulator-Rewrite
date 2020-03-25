#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_DoneSendingInitialEntitiesMsg_Packet : public EQ2Packet {
public:
	OP_DoneSendingInitialEntitiesMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();
	}

private:
	void RegisterElements() {

	}

};
