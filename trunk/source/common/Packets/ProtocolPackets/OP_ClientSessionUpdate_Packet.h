#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_ClientSessionUpdate_Packet : public ProtocolPacket {
public:
	OP_ClientSessionUpdate_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_ClientSessionUpdate;
		HasCRC = true;
	}
	
	uint16_t RequestID;
	uint32_t LastLocalDelta;
	uint32_t AverageDelta;
	uint32_t LowDelta;
	uint32_t HighDelta;
	uint32_t LastRemoteDelta;
	uint64_t PacketsSent;
	uint64_t PacketsRecieved;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt16(RequestID));
		elements.push_back(new PacketUInt32(LastLocalDelta));
		elements.push_back(new PacketUInt32(AverageDelta));
		elements.push_back(new PacketUInt32(LowDelta));
		elements.push_back(new PacketUInt32(HighDelta));
		elements.push_back(new PacketUInt32(LastRemoteDelta));
		elements.push_back(new PacketUInt64(PacketsSent));
		elements.push_back(new PacketUInt64(PacketsRecieved));
	}
};