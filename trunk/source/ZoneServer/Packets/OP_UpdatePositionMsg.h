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
			RegisterUInt32(positionState2);
		}
		else {
			RegisterUInt8(movementMode);
		}
		RegisterFloat(desiredHeading);
		RegisterFloat(desiredPitch);
		if (GetVersion() > 283) {
			RegisterFloat(unusedUnknown);
		}
		RegisterFloat(desiredHeadingVelocity);
		RegisterFloat(desiredPitchVelocity);
		RegisterFloat(collisionRadius);
		RegisterFloat(size);
		RegisterFloat(sizeRatio);
		RegisterFloat(speedModifier);
		if (GetVersion() > 283) {
			RegisterFloat(airSpeed);
			RegisterFloat(desiredStrafeSpeed);
			RegisterFloat(desiredVertSpeed);
			RegisterFloat(unkSpeed3);
		}
		RegisterFloat(swimmingSpeedMultiplier);
		RegisterFloat(desiredForwardSpeed);
		if (GetVersion() > 283) {
			RegisterFloat(desiredRoll);
			RegisterFloat(desiredRollVelocity);
		}
		else {
			RegisterFloat(desiredStrafeSpeed);
			RegisterFloat(desiredVertSpeed);
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

		auto controller = client->GetController();

		//If the crc has not changed since our last update don't bother decoding the data
		if (!controller->CheckPredictionCrc(crc)) {
			return;
		}

		auto xorBuf = client->encoded_packets.GetBuffer(EEncoded_UpdatePosition);
		if (!movement.DecodeData(xorBuf)) {
			LogDebug(LOG_PACKET, 0, "Could not Decode an UpdatePositionMsg?");
			return;
		}

		//Convert heading to a value we recognize
		movement.heading += 180.f;
		movement.desiredHeading += 180.f;
		if (movement.heading >= 360.f) {
			movement.heading -= 360.f;
		}
		if (movement.desiredHeading >= 360.f) {
			movement.desiredHeading -= 360.f;
		}

		if (movement.positionState & POS_STATE_ON_TRANSPORT) {
			auto transportSpawn = client->GetSpawnByID(movement.grid_id);
			if (!transportSpawn) {
				return;
			}
			movement.grid_id = transportSpawn->GetGridID();

			movement.x = transportSpawn->GetX() - movement.x;
			movement.y = transportSpawn->GetY() - movement.y;
			movement.z = transportSpawn->GetZ() - movement.z;
			movement.heading = transportSpawn->GetHeading() - movement.heading;
			movement.desiredHeading = movement.heading;

			//TODO: Save transport spawn to the player's spawn
		}
		
		controller->ApplyPredictionUpdate(timestamp, static_cast<const SpawnPositionStruct&>(movement));
	}

	PacketPackedData packedData;
	Substruct_MovementData movement;

	uint32_t timestamp;
	uint32_t crc;
	uint16_t spawn_index;

private:
	void RegisterElements() {
		RegisterOversizedByte(spawn_index);
		RegisterUInt32(timestamp);
		RegisterUInt32(crc);
		RegisterSubstruct(packedData);
	}
};