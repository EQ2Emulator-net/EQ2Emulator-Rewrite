#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_ChatFiltersMsg_Packet : public EQ2Packet {
public:
	OP_ChatFiltersMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	class Substruct_ChatFilter : public PacketSubstruct {
	public:
		Substruct_ChatFilter(uint32_t ver = 0) : PacketSubstruct(ver, true) { }

		void RegisterElements() override {
			RegisterUInt8(channel);
		}

		//if not equal to 0xFF this channel is not filtered
		uint8_t channel;
	};

	uint16_t filterCount;
	std::vector<Substruct_ChatFilter> filters;

	void RegisterElements() {
		auto e = RegisterUInt16(filterCount);
		e->SetMyArray(RegisterArray(filters, Substruct_ChatFilter));
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_CHAT, 0, "Received a chat filters msg! Handle this?");
	}
};