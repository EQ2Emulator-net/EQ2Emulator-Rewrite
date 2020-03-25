#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_MoveableObjectPlacementCriteri_Packet : public EQ2Packet {
public:
	OP_MoveableObjectPlacementCriteri_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		unknown = 0;
		unknown1 = 0;
		unknown2 = 0;
	}

	float unknown;
	float unknown1;
	float unknown2;

private:
	void RegisterElements() {
		RegisterFloat(unknown);
		RegisterFloat(unknown1);
		RegisterFloat(unknown2);
	}

};

/*
<Struct Name="WS_MoveableObjectPlacementCriteri" ClientVersion="1" OpcodeName="OP_MoveableObjectPlacementCriteri">
<Data ElementName="unknown" Type="int32" Size="1" />
<Data ElementName="unknown1" Type="int32" Size="1" />
<Data ElementName="unknown2" Type="int32" Size="1" />
</Struct>
*/