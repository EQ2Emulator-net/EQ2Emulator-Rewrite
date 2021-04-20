#pragma once

#include "Stream.h"
#include <stdint.h>
#include <deque>
#include <zlib.h>
#include "Crypto.h"
#include "Packets/EQ2Packet.h"
#include "Mutex.h"
#include <map>
#include <atomic>

class ProtocolPacket;

#define FLAG_COMPRESSED	0x01
#define FLAG_ENCODED	0x04

#define RATEBASE	1048576 // 1 MB
#define DECAYBASE	78642	// RATEBASE/10

enum class EQStreamState {
	ESTABLISHED,
	CLOSING,
	CLOSED,
	CLIENTCONFIRMEDCLOSED
};

class EQ2Stream : public Stream, public std::enable_shared_from_this<EQ2Stream> {
public:
	EQ2Stream(unsigned int ip, unsigned short port);
	~EQ2Stream();

	void Process(const unsigned char* data, unsigned int length) override;

	void EQ2QueuePacket(EQ2Packet* app, bool bDelete = true);

	inline EQStreamState GetState() { return State; }
	inline void SetState(EQStreamState state) { State = state; }

	bool CheckActive() { return GetState() == EQStreamState::ESTABLISHED; }
	bool CheckClosed() { return GetState() == EQStreamState::CLOSED; }

	void PreparePacket(EQ2Packet* app, uint8_t offset = 0);

	void Write();

	void SetVersion(uint32_t version) { ClientVersion = version; }
	uint32_t GetVersion() { return ClientVersion; }

	void QueuePacket(EQ2Packet* packet, bool bDelete = true, bool bDump = false);
	void QueuePacket(EQ2Packet& packet, bool bDump = false);

	bool RequestNewClient() override { return bNeedNewClient; }

	void SendDisconnect(uint16_t reason);
	virtual void Disconnected() { }

	uint32_t GetSessionID() { return Session; }

protected:
	EQ2Packet* PopPacket(); // InboundQueuePop
	virtual void ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) = 0;

	uint32_t Key;
	uint32_t Session;
	uint32_t MaxLength;
	//NextInSeq is the next expected sequenced incoming packet
	std::atomic<uint16_t> NextInSeq;
	//NextOutSeq is the next sequenced packet in the queue we will be sending
	uint16_t NextOutSeq;
	int32_t MaxAckReceived;
	//NextAckToSend is read/written to by both threads often so making it atomic
	std::atomic<int32_t> NextAckToSend;
	int32_t LastAckSent;
	int32_t RateThreshold;
	int32_t DecayRate;
	int32_t BytesWritten;
	EQStreamState State;

	uint32_t ClientVersion; // public in old code
	uint8_t	CompressedOffset; //public in old code

	bool bNeedNewClient;

private:
	void ProcessPacket(ProtocolPacket* p);
	void ProcessFuturePacketQueue(int32_t oldSeq, int32_t nextSeq);
	bool ValidateCRC(const unsigned char* buffer, uint16_t length, uint32_t key);
	void EncryptPacket(EQ2Packet* app, uint8_t compress_offset, uint8_t offset);
	void SequencedPush(EQ2Packet* p);
	ProtocolPacket* SequencedPop();
	void NonSequencedPush(ProtocolPacket* p);
	void WritePacket(ProtocolPacket* p);
	uint8_t EQ2_Compress(EQ2Packet* app, uint8_t offset = 3);
	void SetMaxAckReceived(uint16_t seq);
	void SetLastAckSent(int32_t seq);
	void AdjustRates(uint32_t average_delta);
	uint16_t processRSAKey(ProtocolPacket* p);
	bool HandleEmbeddedPacket(unsigned char* p, uint32_t length, bool bFromAppCombine = false);
	EQ2Packet* ProcessEncryptedData(unsigned char* data, uint32_t size);
	EQ2Packet* ProcessEncryptedPacket(ProtocolPacket *p);
	void InboundQueuePush(EQ2Packet* p);
	void InboundQueueClear();
	bool CheckSequencedPacket(ProtocolPacket* p);
	void ProcessFragmentedData(ProtocolPacket* p);

	// Send functions
	void SendAck(uint16_t seq);
	void SendSessionResponse();
	void SendKeepAlive();
	void SendServerSessionUpdate(uint16_t requestID);
	void SendKeyRequest();

	static const char* GetDisconnectReasonString(uint16_t reason);

	Crypto crypto;
	bool Compressed;
	bool Encoded;
	bool bSentKeyRequest;
	z_stream stream;
	uint32_t oversize_offset;
	uint32_t oversize_length;
	unsigned char *oversize_buffer;

	deque<ProtocolPacket*> NonSequencedQueue;
	deque<EQ2Packet*> SequencedQueue;
	deque<ProtocolPacket*> ResendQueue;
	// Packes waiting to be processed
	deque<EQ2Packet *> InboundQueue;
	Mutex inboundQueueLock;
	Mutex seqQueueLock;
	Mutex nonSeqQueueLock;
	Mutex resendQueueLock;


	//This map is only used regularly by the reader thread so it is fine without a mutex
	map<uint16_t, unique_ptr<ProtocolPacket> > FuturePackets;
	deque<ProtocolPacket*> fragmentedQueue;
};