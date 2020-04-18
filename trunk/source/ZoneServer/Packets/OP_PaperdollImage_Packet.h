#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_PaperdollImage_Packet : public EQ2Packet {
public:
	OP_PaperdollImage_Packet(uint32_t version) : EQ2Packet(version) {
		unknown2 = 0;
		RegisterElements();
	}
	~OP_PaperdollImage_Packet() = default;

	void RegisterElements() {
		RegisterUInt8(packetIndex);
		RegisterUInt8(totalNumPackets);
		RegisterUInt64(unknown);
		RegisterUInt8(imageType);
		if (GetVersion() < 60000) {
			RescopeToReference(unknown2, uint16_t);
			RegisterUInt16(unknown2);
		}
		else {
			RegisterUInt32(unknown2);
		}
		Register32String(pngData);
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_PACKET, 0, "Received a paperdoll image. Handle this eventually!");
	}

	uint8_t packetIndex;
	uint8_t totalNumPackets;
	uint64_t unknown;
	uint8_t imageType;
	uint32_t unknown2;
	std::string pngData;
};