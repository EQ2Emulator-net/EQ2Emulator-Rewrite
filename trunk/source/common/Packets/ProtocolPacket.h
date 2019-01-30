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
	uint16_t GetSequence() { return sequence; }
	void SetSequence(uint16_t seq) { sequence = seq; }
	uint16_t GetVersion() { return version; }
	void SetVersion(uint16_t new_version) { version = new_version; }
	uint32_t GetSentTime() { return SentTime; }
	void SetSentTime(uint32_t time) { SentTime = time; }
	//bool Combine(const ProtocolPacket *rhs);

	static ProtocolPacket* GetProtocolPacket(unsigned char* in_buff, uint32_t len);
	static uint32_t Compress(const unsigned char *buffer, const uint32_t length, unsigned char *newbuf, uint32_t newbufsize);
	static void ChatDecode(unsigned char *buffer, int size, int DecodeKey);
	static void ChatEncode(unsigned char *buffer, int size, int EncodeKey);

protected:
	uint16_t opcode;
	uint16_t sequence;
	uint16_t version;
	bool HasCRC;
	uint32_t SentTime;

private:

};