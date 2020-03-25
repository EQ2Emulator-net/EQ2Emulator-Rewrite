#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"

class OP_ClientCmdMsg_Packet : public EQ2Packet {
public:
	OP_ClientCmdMsg_Packet(uint32_t version) : EQ2Packet(version), subOpcode(0xFFFF) {
		RegisterUInt32(clientCmdSize);
		RegisterOversizedByte(subOpcode);
	}
	~OP_ClientCmdMsg_Packet() = default;

	uint16_t GetSubOpcode() {
		return subOpcode;
	}

	void SetSubOpcode(uint16_t op) {
		subOpcode = op;
	}

	EQ2Packet* GetSubPacket() override {
		if (typeid(*this) != typeid(OP_ClientCmdMsg_Packet)) return nullptr;
		return OpcodeManager::GetGlobal()->GetPacketForVersion(GetVersion(), subOpcode);
	}

	uint32_t Write(unsigned char*& writeBuffer) override {
		FindOpcode();
		uint32_t size = GetElementSize();
		clientCmdSize = size - 4;
		return Packet::Write(writeBuffer, size);
	}

private:
	uint32_t clientCmdSize;
	uint16_t subOpcode;
};