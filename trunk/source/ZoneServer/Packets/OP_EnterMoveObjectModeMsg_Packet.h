#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_EnterMoveObjectModeMsg_Packet : public EQ2Packet {
public:
	OP_EnterMoveObjectModeMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	uint32_t spawn_id;
	uint8_t placement_mode;
	uint16_t model_type;
	float unknown;
	float max_distance;
	uint8_t unknown4;

private:
	void RegisterElements() {
		
	}

};

/*
<Struct Name="WS_MoveObjectMode" ClientVersion="1" OpcodeName="OP_EnterMoveObjectModeMsg" >
<Data ElementName="spawn_id" Type="int32" />
<Data ElementName="placement_mode" Type="int8" />
<Data ElementName="model_type" Type="int16" />
<Data ElementName="unknown" Type="float" />
<Data ElementName="max_distance" Type="float" />
<Data ElementName="unknown4" Type="int8" />
</Struct>
<Struct Name="WS_MoveObjectMode" ClientVersion="936" OpcodeName="OP_EnterMoveObjectModeMsg" >
<Data ElementName="spawn_id" Type="int32" />
<Data ElementName="placement_mode" Type="int8" />
<Data ElementName="model_type" Type="int16" />
<Data ElementName="unknown" Type="float" />
<Data ElementName="unknown2" Type="float" Size="4" />
<Data ElementName="max_distance" Type="float" />
<Data ElementName="unknown4" Type="int8" />
</Struct>
<Struct Name="WS_MoveObjectMode" ClientVersion="1193" OpcodeName="OP_EnterMoveObjectModeMsg" >
<Data ElementName="spawn_id" Type="int32" />
  <Data ElementName="CoEunknown" Type="int32" />
<Data ElementName="placement_mode" Type="int8" />
<Data ElementName="model_type" Type="int16" />
  <Data ElementName="CoEunknown2" Type="int8" />
<Data ElementName="unknown" Type="float" />
<Data ElementName="unknown2" Type="float" Size="4" />
<Data ElementName="max_distance" Type="float" />
<Data ElementName="unknown4" Type="int8" />
</Struct>
<Struct Name="WS_MoveObjectMode" ClientVersion="57107" OpcodeName="OP_EnterMoveObjectModeMsg" >
<Data ElementName="spawn_id" Type="int32" />
<Data ElementName="CoEunknown" Type="int32" />
<Data ElementName="placement_mode" Type="int8" />
<Data ElementName="model_type" Type="int16" />
<Data ElementName="CoEunknown2" Type="int8" />
<Data ElementName="unknown3" Type="int16" />
<Data ElementName="unknown" Type="float" />
<Data ElementName="unknown2" Type="float" Size="4" />
<Data ElementName="max_distance" Type="float" />
<Data ElementName="unknown4" Type="int8" />
</Struct>
<Struct Name="WS_MoveObjectMode" ClientVersion="63119" OpcodeName="OP_EnterMoveObjectModeMsg" >
<Data ElementName="spawn_id" Type="int32" />
<Data ElementName="CoEunknown" Type="int32" />
<Data ElementName="placement_mode" Type="int8" />
<Data ElementName="model_type" Type="int16" />
<Data ElementName="CoEunknown2" Type="int8" />
<Data ElementName="unknown3" Type="int16" />
<Data ElementName="unknown" Type="float" />
<Data ElementName="unknown2" Type="float" Size="4" />
<Data ElementName="max_distance" Type="float" />
<Data ElementName="unknown4" Type="int8" />
<Data ElementName="unknown4a" Type="int8" />
<Data ElementName="model_type2" Type="int16" />
<Data ElementName="unknown5" Type="int32" Size="99" />
<Data ElementName="unknown6" Type="int8" />
<Data ElementName="unknown7" Type="int8" />
<Data ElementName="unknown8" Type="int8" />
<Data ElementName="unknown9" Type="int8" />
<Data ElementName="unknown10" Type="int8" />
<Data ElementName="unknown11" Type="int8" />
<Data ElementName="model_type3" Type="int16" />
<Data ElementName="unknown14" Type="int32" Size="20" />
</Struct>

packet->setDataByName("placement_mode", placement_mode);
					packet->setDataByName("spawn_id", client->GetPlayer()->GetIDWithPlayerSpawn(target));
					packet->setDataByName("model_type", target->GetModelType());
					packet->setDataByName("unknown", 1); //size
					packet->setDataByName("unknown2", 1); //size 2
					packet->setDataByName("unknown2", .5, 1); //size 3
					packet->setDataByName("unknown2", 3, 2);
					packet->setDataByName("unknown2", unknown2_3, 3);
					packet->setDataByName("max_distance", 500);
					packet->setDataByName("CoEunknown", 0xFFFFFFFF);
					client->QueuePacket(packet->serialize());
					safe_delete(packet);
*/