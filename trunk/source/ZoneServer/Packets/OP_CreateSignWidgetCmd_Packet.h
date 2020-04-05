#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../Spawns/SpawnStructs.h"
#include "Substruct_SpawnHeader.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "OP_CreateWidgetCmd_Packet.h"

class OP_CreateSignWidgetCmd_Packet : public OP_CreateWidgetCmd_Packet {
public:
	OP_CreateSignWidgetCmd_Packet(uint32_t version) : OP_CreateWidgetCmd_Packet(version), signData(version) {
		RegisterElements();
	}
	~OP_CreateSignWidgetCmd_Packet() = default;

	void RegisterElements() {
		RegisterSubstruct(signData);
	}

	Substruct_SignFooter signData;
};