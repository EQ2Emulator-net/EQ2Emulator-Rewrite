#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../../common/Packets/PacketElements/PacketEncodedData.h"

struct Substruct_MovementData : public PacketEncodedData {
	Substruct_MovementData(uint32_t version) : PacketEncodedData(version) {
		unk_speed = 0.f;
		memset(unknown3 + 8, 0, sizeof(float) * 4);
		RegisterElements();
	}

	uint32_t activity;
	float unknown2;
	float direction1;
	float unknown3[12];
	float unk_speed;
	float speed;
	float side_speed;
	float unknown4;
	float orig_x;
	float orig_y;
	float orig_z;
	float orig_x2;
	float orig_y2;
	float orig_z2;
	float unknown5[3];
	uint32_t unknown6;
	float unknown7[3];
	uint32_t unknown8;
	uint32_t grid_location;
	float x;
	float y;
	float z;

	float direction2;
	float pitch;
	float unknown10;
	float speed_x;
	float speed_y;
	float speed_z;

	void RegisterElements() override {
		RegisterUInt32(activity);
		RegisterFloat(unknown2);
		RegisterFloat(direction1);
		RescopeArrayElement(unknown3);
		RegisterFloat(unknown3)->SetCount(version >= 1144 ? 12 : 8);
		if (version >= 1096) {
			RegisterFloat(unk_speed);
		}
		RegisterFloat(speed);
		RegisterFloat(side_speed);
		RegisterFloat(unknown4);
		RegisterFloat(orig_x);
		RegisterFloat(orig_y);
		RegisterFloat(orig_z);
		RegisterFloat(orig_x2);
		RegisterFloat(orig_y2);
		RegisterFloat(orig_z2);
		
		RescopeArrayElement(unknown5);
		RegisterFloat(unknown5)->SetCount(3);
		RegisterUInt32(unknown6);
		RescopeArrayElement(unknown7);
		RegisterFloat(unknown7)->SetCount(3);
		RegisterUInt32(unknown8);
		RegisterUInt32(grid_location);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
		RegisterFloat(direction2);
		RegisterFloat(pitch);
		RegisterFloat(unknown10);
		RegisterFloat(speed_x);
		RegisterFloat(speed_y);
		RegisterFloat(speed_z);
	}
};

class OP_UpdatePositionMsg : public EQ2Packet {
public:
	OP_UpdatePositionMsg(uint32_t version) : EQ2Packet(version), movement(version) {
		packedData.LinkSubstruct(movement, "movement");
		RegisterElements();
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		if (packedData.lastPackedSize == 0) {
			return;
		}

		auto xorBuf = client->encoded_packets.GetBuffer(EEncoded_UpdatePosition);
		if (!movement.DecodeData(xorBuf)) {
			LogDebug(LOG_PACKET, 0, "Could not Decode an UpdatePositionMsg?");
			return;
		}

		LogDebug(LOG_PACKET, 0, "%f, %f, %f", movement.x, movement.y, movement.z);
	}

	PacketPackedData packedData;
	Substruct_MovementData movement;

	uint64_t timestamp;
	uint16_t spawn_index;

private:
	void RegisterElements() {
		RegisterOversizedByte(spawn_index);
		RegisterUInt64(timestamp);
		RegisterSubstruct(packedData);
	}
};