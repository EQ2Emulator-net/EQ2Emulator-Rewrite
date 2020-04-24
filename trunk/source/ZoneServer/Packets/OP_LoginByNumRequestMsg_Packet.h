#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator g_zoneOperator;

class OP_LoginByNumRequestMsg_Packet : public EQ2Packet {
public:
	OP_LoginByNumRequestMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
		version = 0;
	}
	~OP_LoginByNumRequestMsg_Packet() = default;

	bool Read(const unsigned char* in_buf, uint32_t off, uint32_t bufsize) override {
		bool ret = Packet::Read(in_buf, off, bufsize);
		if (GetVersion() >= 1212) {
			//We were just using 2 bytes of this dateTime before we knew what it was as a version
			//It rolled over to 0x2F00000000, factor the value over that rollover into our new version
			uint32_t rolloverByte = (version >> 32) & 0xFF;
			uint32_t additiveValue = (rolloverByte - 0x2E) << 16;

			//Move the 2 bytes we want all the way over
			version >>= 16;
			//Strip off any higher bits
			version &= 0xFFFF;
			//Add any extra rollover value
			version |= additiveValue;
		}
		return ret;
	}

	void HandlePacket(std::shared_ptr<Client> client) {
		LogError(LOG_PACKET, 0, "Client with account id of %u connected", account_id);
		g_zoneOperator.ClientLogIn(client, this);
	}

	uint32_t account_id;
	uint32_t access_code;
	uint32_t unknown;
	uint64_t version;

private:
	void RegisterElements() {
		RegisterUInt32(account_id);
		RegisterUInt32(access_code);
		if (GetVersion() > 283) {
			RegisterUInt32(unknown);
		}

		if (GetVersion() < 1212) {
			uint16_t& version = reinterpret_cast<uint16_t&>(this->version);
			RegisterUInt16(version);
		}
		else {
			RegisterUInt64(version);
		}
	}
};