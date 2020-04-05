#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../Spawns/SpawnStructs.h"
#include "Substruct_SpawnHeader.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "OP_CreateGhostCmd_Packet.h"

class OP_CreateWidgetCmd_Packet : public OP_CreateGhostCmd_Packet {
public:
	OP_CreateWidgetCmd_Packet(uint32_t version) : OP_CreateGhostCmd_Packet(version),
		widgetData(version)
	{
		RegisterElements();
	}
	~OP_CreateWidgetCmd_Packet() = default;

	void RegisterElements() {
		RegisterSubstruct(widgetData);
	}

	Substruct_WidgetFooter widgetData;
};