#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"

class OP_ClientCmdMsg_Packet : public EQ2Packet {
public:
	OP_ClientCmdMsg_Packet(uint32_t version) : EQ2Packet(version), subOpcode(0xFFFF) {
		clientCmdSize = 0;
		RegisterElements();
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
		if (GetVersion() > 283) {
			clientCmdSize = size - 4;
		}
		else if (size - 1 >= 255) {
			//Fix the oversized client command size bytes
			size += 2;
			clientCmdSize = size - 1;
		}
		return Packet::Write(writeBuffer, size);
	}

	void RegisterElements() {
		if (GetVersion() > 283) {
			RegisterUInt32(clientCmdSize)->SetIsHidden(true);
		}
		else {
			uint16_t& clientCmdSize = reinterpret_cast<uint16_t&>(this->clientCmdSize);
			RegisterOversizedByte(clientCmdSize)->SetIsHidden(true);
		}
		RegisterOversizedByte(subOpcode)->SetIsHidden(true);
	}

private:
	uint32_t clientCmdSize;
	uint16_t subOpcode;
};