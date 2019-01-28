#pragma once

#include "Packet.h"

static const char OP_SessionRequest			= 0x01;
static const char OP_SessionResponse		= 0x02;
static const char OP_Combined				= 0x03;
static const char OP_SessionDisconnect		= 0x05;
static const char OP_KeepAlive				= 0x06;
static const char OP_ClientSessionUpdate	= 0x07;
static const char OP_ServerSessionUpdate	= 0x08;
static const char OP_Packet					= 0x09;
static const char OP_Fragment				= 0x0D;
static const char OP_OutOfOrderAck			= 0x11;
static const char OP_Ack					= 0x15;
static const char OP_AppCombined			= 0x19;
static const char OP_OutOfSession			= 0x1D;

class ProtocolPacket : public Packet {
public:
	ProtocolPacket();

	uint16_t GetOpcode() { return opcode; }
	uint32_t Write(unsigned char*& writeBuffer) override;

	static ProtocolPacket* GetProtocolPacket(unsigned char* in_buff, uint32_t len);

protected:
	uint16_t opcode;
	bool HasCRC;

private:

};