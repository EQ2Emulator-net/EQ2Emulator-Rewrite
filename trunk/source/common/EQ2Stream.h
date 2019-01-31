#pragma once

#include "Stream.h"
#include <stdint.h>
#include <deque>
#include <zlib.h>
#include "Crypto.h"
#include "Packets/EQ2Packet.h"

class ProtocolPacket;

#define FLAG_COMPRESSED	0x01
#define FLAG_ENCODED	0x04

#define RATEBASE	1048576 // 1 MB
#define DECAYBASE	78642	// RATEBASE/10

typedef enum {
	ESTABLISHED,
	CLOSING,
	CLOSED
} EQStreamState;

typedef enum {
	UnknownStream = 0,
	LoginStream,
	WorldStream,
	ZoneStream,
	ChatOrMailStream,
	ChatStream,
	MailStream,
	EQ2Stream,
} EQStreamType;

class EQ2Stream : public Stream {
public:
	EQ2Stream(unsigned int ip, unsigned short port);
	~EQ2Stream();

	void Process(unsigned char* data, unsigned int length) override;

	void EQ2QueuePacket(EQ2Packet* app, bool attempted_combine = false);

	inline EQStreamState GetState() { return State; }
	inline void SetState(EQStreamState state) { State = state; }

	bool CheckActive() { return GetState() == ESTABLISHED; }
	bool CheckClosed() { return GetState() == CLOSED; }

	void PreparePacket(EQ2Packet* app, uint8_t offset = 0);

	void Write();

	void SetVersion(uint16_t version) { ClientVersion = version; }
	uint16_t GetVersion() { return ClientVersion; }

	std::deque<EQ2Packet*> combine_queue; // public in old code?

protected:
	EQ2Packet* PopPacket(); // InboundQueuePop

	uint32_t Key;
	uint32_t Session;
	uint32_t MaxLength;
	uint16_t NextInSeq;
	uint16_t NextOutSeq;
	int32_t MaxAckReceived;
	int32_t NextAckToSend;
	int32_t LastAckSent;
	int32_t LastSeqSent;
	int32_t RateThreshold;
	int32_t DecayRate;
	int32_t BytesWritten;

	EQ2Packet* CombinedAppPacket;
	EQStreamState State;

	uint16_t ClientVersion; // public in old code
	uint8_t	CompressedOffset; //public in old code

private:
	void ProcessPacket(ProtocolPacket* p);
	bool ValidateCRC(unsigned char* buffer, uint16_t length, uint32_t key);
	void EncryptPacket(EQ2Packet* app, uint8_t compress_offset, uint8_t offset);
	void SendPacket(EQ2Packet* p);
	void SequencedPush(ProtocolPacket* p);
	void NonSequencedPush(ProtocolPacket* p);
	void WritePacket(ProtocolPacket* p);
	uint8_t EQ2_Compress(EQ2Packet* app, uint8_t offset = 3);
	void SetMaxAckReceived(uint32_t seq);
	void SetLastAckSent(int32_t seq);
	void AdjustRates(uint32_t average_delta);
	int8_t CompareSequence(uint16_t expected_seq, uint16_t seq);
	void SetNextAckToSend(uint32_t seq);
	uint16_t processRSAKey(ProtocolPacket *p);
	bool HandleEmbeddedPacket(ProtocolPacket* p, uint16_t offset = 2, uint16_t length = 0);
	EQ2Packet* ProcessEncryptedData(unsigned char* data, uint32_t size, uint16_t opcode);
	EQ2Packet* ProcessEncryptedPacket(ProtocolPacket *p);
	void InboundQueuePush(EQ2Packet* p);
	void InboundQueueClear();

	// Send functions
	void SendAck(uint16_t seq);
	void SendSessionResponse();
	void SendDisconnect(uint16_t reason);
	void SendKeepAlive();
	void SendServerSessionUpdate(uint16_t requestID);
	void SendKeyRequest();

	

	Crypto crypto;
	bool Compressed;
	bool Encoded;
	z_stream stream;

	deque<ProtocolPacket*> NonSequencedQueue;
	deque<ProtocolPacket*> SequencedQueue;
	deque<ProtocolPacket*> ResendQueue;
	// Packes waiting to be processed
	deque<EQ2Packet *> InboundQueue;

};