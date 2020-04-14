#pragma once

#include "Stream.h"
#include <stdint.h>
#include <deque>
#include <zlib.h>
#include "Crypto.h"
#include "Packets/EQ2Packet.h"
#include "Mutex.h"
#include <map>

class ProtocolPacket;

#define FLAG_COMPRESSED	0x01
#define FLAG_ENCODED	0x04

#define RATEBASE	1048576 // 1 MB
#define DECAYBASE	78642	// RATEBASE/10

enum class EQStreamState {
	ESTABLISHED,
	CLOSING,
	CLOSED
};

class EQ2Stream : public Stream, public std::enable_shared_from_this<EQ2Stream> {
public:
	EQ2Stream(unsigned int ip, unsigned short port);
	~EQ2Stream();

	void Process(const unsigned char* data, unsigned int length) override;
	void ProcessFuturePacketQueue();

	void EQ2QueuePacket(EQ2Packet* app, bool attempted_combine = false, bool bDelete = true);

	inline EQStreamState GetState() { return State; }
	inline void SetState(EQStreamState state) { State = state; }

	bool CheckActive() { return GetState() == EQStreamState::ESTABLISHED; }
	bool CheckClosed() { return GetState() == EQStreamState::CLOSED; }

	void PreparePacket(EQ2Packet* app, uint8_t offset = 0);

	void Write();

	void SetVersion(uint32_t version) { ClientVersion = version; }
	uint32_t GetVersion() { return ClientVersion; }

	void QueuePacket(EQ2Packet* packet, bool bDelete = true);
	void QueuePacket(EQ2Packet& packet);

	bool RequestNewClient() override { return bNeedNewClient; }

	std::deque<EQ2Packet*> combine_queue; // public in old code?

protected:
	EQ2Packet* PopPacket(); // InboundQueuePop
	virtual void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) = 0;

	uint32_t Key;
	uint32_t Session;
	uint32_t MaxLength;
	//NextInSeq is the next expected sequenced incoming packet
	uint16_t NextInSeq;
	//NextOutSeq is the next sequenced packet in the queue we will be sending
	uint16_t NextOutSeq;
	//NextAddSeq is the next sequence we want to queue
	int32_t NextAddSeq;
	uint16_t MaxAckReceived;
	int32_t NextAckToSend;
	uint16_t LastAckSent;
	uint16_t LastSeqSent;
	int32_t RateThreshold;
	int32_t DecayRate;
	int32_t BytesWritten;

	EQ2Packet* CombinedAppPacket;
	EQStreamState State;

	uint32_t ClientVersion; // public in old code
	uint8_t	CompressedOffset; //public in old code

	bool bNeedNewClient;

private:
	void ProcessPacket(ProtocolPacket* p);
	bool ValidateCRC(const unsigned char* buffer, uint16_t length, uint32_t key);
	void EncryptPacket(EQ2Packet* app, uint8_t compress_offset, uint8_t offset);
	void SendPacket(EQ2Packet* p, bool bDelete = true);
	void SequencedPush(ProtocolPacket* p);
	void NonSequencedPush(ProtocolPacket* p);
	void WritePacket(ProtocolPacket* p);
	uint8_t EQ2_Compress(EQ2Packet* app, uint8_t offset = 3);
	void SetMaxAckReceived(uint16_t seq);
	void SetLastAckSent(uint16_t seq);
	void AdjustRates(uint32_t average_delta);
	int8_t CompareSequence(uint16_t expected_seq, uint16_t seq);
	void SetNextAckToSend(int32_t seq);
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
	uint32_t oversize_offset;
	uint32_t oversize_length;
	unsigned char *oversize_buffer;

	deque<ProtocolPacket*> NonSequencedQueue;
	deque<ProtocolPacket*> SequencedQueue;
	deque<ProtocolPacket*> ResendQueue;
	// Packes waiting to be processed
	deque<EQ2Packet *> InboundQueue;
	Mutex inboundQueueLock;
	Mutex seqQueueLock;
	Mutex nonSeqQueueLock;
	Mutex resendQueueLock;

	//This lock is to prevent a race condition where the NextOutSeq variable is unexpectedly changed while we are resending packets
	Mutex NextOutSeqLock;
	//This map is only used regularly by the reader thread so it is fine without a mutex
	map<uint16_t, unique_ptr<ProtocolPacket> > FuturePackets;
};