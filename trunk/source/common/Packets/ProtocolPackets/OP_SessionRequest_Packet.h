#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_SessionRequest_Packet : public ProtocolPacket {
public:
	OP_SessionRequest_Packet() 
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_SessionRequest;
	}

	uint32_t ProtocolVersion;
	uint32_t Session;
	uint32_t MaxLength;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt32(ProtocolVersion));
		elements.push_back(new PacketUInt32(Session));
		elements.push_back(new PacketUInt32(MaxLength));
	}

};