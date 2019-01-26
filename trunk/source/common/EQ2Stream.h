#pragma once

#include "Stream.h"
#include <stdint.h>

class EQ2Stream : public Stream {
public:
	EQ2Stream(unsigned int ip, unsigned short port);

	void Process(unsigned char* data, unsigned int length) override;

protected:
	virtual bool HandlePacket();

	uint32_t Key;
	uint32_t Session;

private:
	void ProcessPacket(unsigned char* data, unsigned int length);
	bool ValidateCRC(unsigned char* buffer, uint16_t length, uint32_t key);

};