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
			RegisterUInt8(filter);
		}

		//every bit is a filter by index in the packet, if the bit is 1 it is not filtered
		//For example if filter id "2" (GameText) is filtered out the first byte in the filter array will be ((filterByte & (1 << 2)) == 0)
		uint8_t filter;
	};

	uint16_t filterCount;
	std::vector<Substruct_ChatFilter> filtersArray;

	void RegisterElements() {
		auto e = RegisterUInt16(filterCount);
		e->SetMyArray(RegisterArray(filtersArray, Substruct_ChatFilter));
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_CHAT, 0, "Received a chat filters msg! Handle it!");
	}
};