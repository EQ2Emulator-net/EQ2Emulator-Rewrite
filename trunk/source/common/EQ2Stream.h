#pragma once

#include "Stream.h"
#include <stdint.h>

class ProtocolPacket;

class EQ2Stream : public Stream {
public:
	EQ2Stream(unsigned int ip, unsigned short port);

	void Process(unsigned char* data, unsigned int length) override;

protected:
	virtual bool HandlePacket();

	uint32_t Key;
	uint32_t Session;
	uint32_t MaxLength;
	uint16_t NextInSeq;
	uint16_t NextOutSeq;
	int32_t MaxAckReceived;
	int32_t NextAckToSend;
	int32_t LastAckSent;
	int32_t LastSeqSent;

private:
	void ProcessPacket(ProtocolPacket* p);
	bool ValidateCRC(unsigned char* buffer, uint16_t length, uint32_t key);
	void SendSessionResponse();
	void SendDisconnect(uint16_t reason);
	void SendKeepAlive();
	void SendServerSessionUpdate(uint16_t requestID);

};