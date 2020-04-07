#pragma once

#include "../../common/EQ2Stream.h"
#include "../Packets/EncodedPackets.h"

class Client : public EQ2Stream {
public:
	Client(unsigned int ip, unsigned short port);

	void Process();

	void SendLoginReply(uint8_t reply);

	EncodedPackets encoded_packets;

protected:
	void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) override;

private:

	uint32_t account_id;
	uint32_t character_id;


public:
	void SetAccountID(uint32_t val) { account_id = val; }
	uint32_t GetAccountID() { return account_id; }

	void SetCharacterID(uint32_t val) { character_id = val; }
	uint32_t GetCharacterID() { return character_id; }
};