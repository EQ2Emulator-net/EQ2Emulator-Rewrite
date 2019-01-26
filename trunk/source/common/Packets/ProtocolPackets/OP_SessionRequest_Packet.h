#pragma once

#include "../ProtocolPacket.h"

#include "../PacketElements/PacketUInt32.h"

class OP_SessionRequest_Packet : public ProtocolPacket {
public:
	OP_SessionRequest_Packet() 
		: ProtocolPacket() {
		RegisterElements();
	}

	uint32_t UnknownA;
	uint32_t Session;
	uint32_t MaxLength;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt32(UnknownA));
		elements.push_back(new PacketUInt32(Session));
		elements.push_back(new PacketUInt32(MaxLength));
	}

};