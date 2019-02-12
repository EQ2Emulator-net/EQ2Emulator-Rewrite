#pragma once

#include "../../common/EQ2Stream.h"

class Client : public EQ2Stream {
public:
	Client(unsigned int ip, unsigned short port);

	void Process();

protected:
	void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) override;

private:

};