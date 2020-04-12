#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../../common/Packets/PacketElements/PacketEncodedData.h"
#include "../Spawns/SpawnStructs.h"
#include "../Controllers/PlayerController.h"
#include "../ZoneServer/Client.h"
#include "../Spawns/Entity.h"

struct Substruct_MovementData : public PacketEncodedData, public SpawnPositionStruct {
	Substruct_MovementData(uint32_t version) : PacketEncodedData(version) {
		RegisterElements();
	}

	void RegisterElements() override {
		RegisterUInt32(positionState);
		if (GetVersion() > 283) {
			RegisterFloat(unknown20);
		}
		RegisterFloat(desiredHeading);
		RegisterFloat(desiredPitch);
		if (GetVersion() > 283) {
			RegisterFloat(unusedUnknown);
		}
		RegisterFloat(desiredHeadingVelocity);
		RegisterFloat(desiredPitchVelocity);
		RegisterFloat(size);
		RegisterFloat(sizeRatio);
		RegisterFloat(sizeMultiplierRatio);
		RegisterFloat(speedModifier);
		RegisterFloat(swimmingSpeedModifier);
		RegisterFloat(desiredStrafeSpeed);
		RegisterFloat(desiredVertSpeed);
		if (GetVersion() > 283) {
			RegisterFloat(unkSpeed3);
			RegisterFloat(moveType);
		}
		RegisterFloat(desiredForwardSpeed);
		if (GetVersion() > 283) {
			RegisterFloat(desiredRoll);
			RegisterFloat(desiredRollVelocity);
		}
		RegisterFloat(destLocX);
		RegisterFloat(destLocY);
		RegisterFloat(destLocZ);
		RegisterFloat(destLocX2);
		RegisterFloat(destLocY2);
		RegisterFloat(destLocZ2);
		if (GetVersion() > 283) {
			RegisterFloat(baseLocX);
			RegisterFloat(baseLocY);
			RegisterFloat(baseLocZ);
		}
		RegisterUInt32(faceActorID);
		RegisterFloat(actorStopRange);
		if (GetVersion() > 283) {
			RegisterFloat(sizeUnknown);
			RegisterFloat(unk70);
			RegisterUInt8(movementMode);
			RegisterUInt8(unkByte);
			static uint16_t align_trash;
			RegisterUInt16(align_trash);
		}
		RegisterUInt32(grid_id);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
		RegisterFloat(heading);
		RegisterFloat(pitch);
		RegisterFloat(roll);
		RegisterFloat(velocityX);
		RegisterFloat(velocityY);
		RegisterFloat(velocityZ);
	}
};

class OP_UpdatePositionMsg : public EQ2Packet {
public:
	OP_UpdatePositionMsg(uint32_t version) : EQ2Packet(version), packedData(version <= 283), movement(version) {
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

		auto controller = client->GetController();
		auto entity = controller->GetControlled();

		if (entity) {
			entity->SetSpawnPositionData(static_cast<const SpawnPositionStruct&>(movement));
		}
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