#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_ServerSessionUpdate_Packet : public ProtocolPacket {
public:
	OP_ServerSessionUpdate_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_ServerSessionUpdate;
		HasCRC = true;

		RequestID = 0;
		CurrentTime = 0;
		Unknown1 = 0;
		ReceivedPackets = 0;
		Unknown2 = 0;
		SentPackets = 0;
		Unknown3 = 0;
		SentPackets2 = 0;
		Unknown4 = 0;
		ReceivedPackets2 = 0;
	}

	uint16_t RequestID;
	uint32_t CurrentTime;
	uint32_t Unknown1;
	uint32_t ReceivedPackets;
	uint32_t Unknown2;
	uint32_t SentPackets;
	uint32_t Unknown3;
	uint32_t SentPackets2;
	uint32_t Unknown4;
	uint32_t ReceivedPackets2;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt16(RequestID));
		elements.push_back(new PacketUInt32(CurrentTime));
		elements.push_back(new PacketUInt32(Unknown1));
		elements.push_back(new PacketUInt32(ReceivedPackets));
		elements.push_back(new PacketUInt32(Unknown2));
		elements.push_back(new PacketUInt32(SentPackets));
		elements.push_back(new PacketUInt32(Unknown3));
		elements.push_back(new PacketUInt32(SentPackets2));
		elements.push_back(new PacketUInt32(Unknown4));
		elements.push_back(new PacketUInt32(ReceivedPackets2));
	}
};