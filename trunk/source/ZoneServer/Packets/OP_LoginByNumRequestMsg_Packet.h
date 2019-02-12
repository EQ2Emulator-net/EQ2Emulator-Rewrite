#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_LoginByNumRequestMsg_Packet : public EQ2Packet {
public:
	OP_LoginByNumRequestMsg_Packet(uint16_t version) : EQ2Packet(version) {
		RegisterElements();
		unknown2[5] = 0;
	}
	~OP_LoginByNumRequestMsg_Packet() = default;

	uint32_t account_id;
	uint32_t access_code;
	uint32_t unknown;
	uint16_t unknown1212;
	uint16_t version;
	uint32_t unknown2[6];

private:
	void RegisterElements() {
		RegisterUInt32(account_id);
		RegisterUInt32(access_code);
		RegisterUInt32(unknown);

		if (GetVersion() >= 1212) {
			RegisterUInt16(unknown1212);
		}

		RegisterUInt16(version);
		
		uint32_t& unknown2 = this->unknown2[0];

		RegisterUInt32(unknown2)->SetCount(GetVersion() < 1212 ? 5 : 6);
	}
};