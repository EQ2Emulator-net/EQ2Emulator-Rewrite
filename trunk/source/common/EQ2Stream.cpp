#include "stdafx.h"

#include "EQ2Stream.h"
#include "log.h"
#include "Packets/ProtocolPackets/ProtocolPackets.h"
#include "CRC16.h"
#include "util.h"
#include "Server.h"
#include "timer.h"
#include "Packets/EQ2Packets/OpcodeManager.h"
#include "../common/Packets/PacketElements/PacketElements.h"

EQ2Stream::EQ2Stream(unsigned int ip, unsigned short port) : Stream(ip, port) {
	Key = 0;
	Session = 0;
	MaxLength = 0;
	NextInSeq = 0;
	NextOutSeq = 0;
	MaxAckReceived = -1;
	NextAckToSend = -1;
	LastAckSent = -1;
	Compressed = false;
	Encoded = false;
	bNeedNewClient = false;
	ClientVersion = 0;

	RateThreshold = RATEBASE / 250;
	DecayRate = DECAYBASE / 250;
	BytesWritten = 0;
	crypto.setRC4Key(0);

	stream.opaque = Z_NULL;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	deflateInit2(&stream, 9, Z_DEFLATED, 13, 9, Z_DEFAULT_STRATEGY);

	oversize_buffer = nullptr;
	oversize_length = 0;
	oversize_offset = 0;
	CompressedOffset = 0;
	State = EQStreamState::CLOSED;
	bSentKeyRequest = false;
}

EQ2Stream::~EQ2Stream() {
	for (auto& itr : NonSequencedQueue) {
		delete itr;
	}
	for (auto& itr : SequencedQueue) {
		delete itr;
	}
	for (auto& itr : fragmentedQueue) {
		delete itr;
	}

	InboundQueueClear();

	deflateEnd(&stream);
}

void EQ2Stream::Process(const unsigned char* data, unsigned int length) {
	Stream::Process(data, length);

	if (length < 2) {
		LogError(LOG_NET, 0, "Received a packet which is too short to process.");
		return;
	}

	if (!ValidateCRC(data, length, Key)) {
		LogError(LOG_NET, 0, "Packet CRC check failed. Client %s", ToString().c_str());
		return;
	}

	ProtocolPacket* p = ProtocolPacket::GetProtocolPacket(data, length, true);
	if (p) {
		int32_t preProcessInSeq = NextInSeq.load();
		ProcessPacket(p);
		delete p;
		int32_t nextSeq = NextInSeq.load();
		if (preProcessInSeq != nextSeq) {
			ProcessFuturePacketQueue(preProcessInSeq, nextSeq);
		}
	}
	//Randomly the client sends an EQ2Packet without a protocol opcode after logging into a zone
	//Attempt to handle it or else our encryption will desync
	else if (ntohs(*reinterpret_cast<const uint16_t*>(data)) > 0x1e) {
		//Possible garbage packet? Try to decrypt it
		LogWarn(LOG_PACKET, 0, "Received an out of protocol packet, trying to process it.");
		uint32_t pSize = length - 2;
		unsigned char* pData = new unsigned char[pSize];
		memcpy(pData, data, pSize);
		if (!HandleEmbeddedPacket(pData, pSize)) {
			EQ2Packet* newpacket = ProcessEncryptedData(pData, pSize);
			if (newpacket) {
				InboundQueuePush(newpacket);
			}
		}
		delete[] pData;
	}
}

void EQ2Stream::ProcessFuturePacketQueue(int32_t oldSeq, int32_t nextSeq) {
	if (FuturePackets.empty()) {
		return;
	}

	//For the most part FuturePackets will be cleared without this but it is possible for one to linger if a future packet is embedded in a combine with more than one sequenced packet
	//Get rid of it to prevent an issue after sequence overflow and prevent a soft memory leak
	while (oldSeq < nextSeq) {
		FuturePackets.erase(oldSeq++);
	}

	for (;;) {
		auto itr = FuturePackets.find(NextInSeq.load());
		if (itr == FuturePackets.end()) {
			break;
		}

		ProcessPacket(itr->second.get());
		FuturePackets.erase(itr);
	}
}

//Return value is true if packet is in sequence
bool EQ2Stream::CheckSequencedPacket(ProtocolPacket* p) {
	uint16_t seq = ntohs(*reinterpret_cast<uint16_t*>(p->buffer));
	uint16_t expected = NextInSeq.load();
	bool ret = false;

	if (NetDebugEnabled()) {
		LogDebug(LOG_PACKET, 0, "seq = %u, NextInSeq = %u", seq, expected);
	}
	if (seq > expected) {
		// Future
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "Future packet");
		}

		FuturePackets[seq].reset(p->MoveCopy());
		NonSequencedPush(new OP_OutOfOrderAck_Packet(seq));
	}
	
	else if (seq < expected) {
		// Past
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "Past packet");
		}
		//20 is arbitrary but there to prevent acking a future packet after a sequence overflow without storing it
		//Extremely rare/unlikely case
		if (seq > 20) {
			NonSequencedPush(new OP_OutOfOrderAck_Packet(seq));
		}
	}
	else {
		NextAckToSend.fetch_add(1);
		NextInSeq.fetch_add(1);
		ret = true;
	}

	return ret;
}

void EQ2Stream::ProcessPacket(ProtocolPacket* p) {
	if (NetDebugEnabled()) {
		LogDebug(LOG_NET, 0, "ProtocolPacket Received, opcode: %u", p->GetOpcode());
	}

	switch (p->GetOpcode()) {
	case OP_SessionRequest: {
		auto request = static_cast<OP_SessionRequest_Packet*>(p);

		if (Session != 0 && ntohl(request->Session) != Session) {
			bNeedNewClient = true;
			return;
		}

		Session = ntohl(request->Session);
		MaxLength = ntohl(request->MaxLength);
		//Setting the sequence to 0 may be a bad idea in case we get multiple of these packets
		//NextInSeq = 0;
		Key = 0x33624702;
		LogDebug(LOG_NET, 3, "OP_SessionRequest Protocol Version: %u, Session: %u, MaxLength: %u", ntohl(request->ProtocolVersion), Session, MaxLength);

		SendSessionResponse();
		SetState(EQStreamState::ESTABLISHED);
		break;
	}
	case OP_SessionDisconnect: {
		auto disconnect = static_cast<OP_SessionDisconnect_Packet*>(p);
		LogDebug(LOG_NET, 0, "Client from %s is disconnecting with reason: %s", ToString().c_str(), GetDisconnectReasonString(disconnect->Reason));
		SendDisconnect(disconnect->Reason);
		break;
	}
	case OP_KeepAlive: {
		SendKeepAlive();
		break;
	}
	case OP_ClientSessionUpdate: {
		auto update = static_cast<OP_ClientSessionUpdate_Packet*>(p);
		AdjustRates(ntohl(update->AverageDelta));
		SendServerSessionUpdate(update->RequestID);
		if (!crypto.isEncrypted() && !bSentKeyRequest) {
			SendKeyRequest();
			bSentKeyRequest = true;
		}
		break;
	}
	case OP_Packet: {
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "OP_Packet_Packet Dump");
			DumpBytes(p->buffer, p->Size);
		}
		if (CheckSequencedPacket(p)) {
			if (HandleEmbeddedPacket(p->buffer + 2, p->Size - 2)) {
				break;
			}
			else if (crypto.getRC4Key() == 0 && p->Size >= 70) {
				processRSAKey(p);
			}
			else if (crypto.isEncrypted()) {
				EQ2Packet* newpacket = ProcessEncryptedPacket(p);
				if (newpacket) {
					InboundQueuePush(newpacket);
				}
			}
		}
		break;
	}
	case OP_Fragment: {
		if (!CheckSequencedPacket(p)) {
			break;
		}
		ProcessFragmentedData(p);
		break;
	}
	case OP_Ack: {
		auto ack = static_cast<OP_Ack_Packet*>(p);
		SetMaxAckReceived(ack->Sequence);
		break;
	}
	case OP_AppCombined: {
		uint32_t processed = 0;
		uint32_t subpacket_length = 0;
		EQ2Packet* newpacket = 0;
		uint32_t offset = 0;
		int count = 0;

		while (processed < p->Size) {
			count++;
			if ((subpacket_length = (unsigned char)*(p->buffer + processed)) == 0xff) {
				subpacket_length = ntohs(*(uint16_t*)(p->buffer + processed + 1));
				offset = 3;
			}
			else
				offset = 1;

			uint32_t dataOffset = processed + offset;

			if (crypto.getRC4Key() == 0 && p->Size >= 70) {
				processRSAKey(p);
			}
			else if (crypto.isEncrypted()) {
				if (dataOffset + subpacket_length > p->Size) {
					LogError(LOG_PACKET, 0, "An OP_AppCombined packet tried to read past the end of its buffer!");
					//DumpBytes(p->buffer, p->Size);
					return;
				}
				unsigned char* dataPtr = p->buffer + dataOffset;
				if (!HandleEmbeddedPacket(dataPtr, subpacket_length)) {
					newpacket = ProcessEncryptedData(dataPtr, subpacket_length);
					if (newpacket) {
						InboundQueuePush(newpacket);
					}
				}
			}
			processed += subpacket_length + offset;
		}
		break;
	}
	case OP_Combined: {
		uint32_t processed = 0;
		uint32_t subpacket_length;

		while (processed < p->Size) {
			subpacket_length = p->buffer[processed++];

			if (subpacket_length == 0) {
				LogError(LOG_PACKET, 0, "Received a bad combine packet from a client! (size == 0)");
				break;
			}

			if (subpacket_length + processed > p->Size) {
				LogError(LOG_PACKET, 0, "Received a bad combine packet from a client! (size > remaining_bytes)");
				break;
			}

			uint32_t dataOffset = processed;
			unsigned char* dataPtr = p->buffer + dataOffset;
			ProtocolPacket* subPacket = ProtocolPacket::GetProtocolPacket(dataPtr, subpacket_length, false);

			if (subPacket) {
				ProcessPacket(subPacket);
				delete subPacket;
			}
			//Randomly the client sends an EQ2Packet without a protocol opcode after logging into a zone
			//Attempt to handle it or else our encryption will desync
			else if (ntohs(*reinterpret_cast<uint16_t*>(dataPtr)) > 0x1e) {
				//Possible garbage packet? Try to decrypt it
				LogWarn(LOG_PACKET, 0, "Received an out of protocol packet in a combine, trying to process it.");
				if (!HandleEmbeddedPacket(dataPtr, subpacket_length)) {
					EQ2Packet* newpacket = ProcessEncryptedData(dataPtr, subpacket_length);
					if (newpacket) {
						InboundQueuePush(newpacket);
					}
				}
			}

			processed += subpacket_length;
		}
		break;
	}
	//This is an ack for a single packet rather than a range of packets
	case OP_OutOfOrderAck: {
		auto oop = static_cast<OP_OutOfOrderAck_Packet*>(p);
		uint16_t seq = oop->Sequence;
		WriteLocker lock(resendQueueLock);
		for (auto itr = ResendQueue.begin(); itr != ResendQueue.end(); ++itr) {
			ProtocolPacket* p = *itr;
			if (p->GetSequence() == seq) {
				ResendQueue.erase(itr);
				delete p;
				break;
			}
		}
		break;
	}
	case OP_OutOfSession: {
		LogError(LOG_NET, 0, "OutOfSession packet");
		break;
	}
	default:
		break;
	}
}

bool EQ2Stream::ValidateCRC(const unsigned char* buffer, uint16_t length, uint32_t key) {
	bool valid = false;
	if (buffer[0] == 0x00 && (buffer[1] == OP_SessionRequest || buffer[1] == OP_SessionResponse || buffer[1] == OP_OutOfSession))
		valid = true;
	else if (buffer[2] == 0x00 && buffer[3] == 0x19)
		valid = true;
	else {
		uint16_t comp_crc = (uint16_t)CRC16(buffer, length - 2, key);
		uint16_t packet_crc = ntohs(*(uint16_t*)(buffer + length - 2));

		valid = (!packet_crc || comp_crc == packet_crc);
	}

	return valid;
}

void EQ2Stream::WritePacket(ProtocolPacket* p) {
	uint32_t size;
	unsigned char* buffer;
	unsigned char tmpbuffer[1024];
	if (p->bBufferSet) {
		buffer = p->buffer;
		size = p->Size;
	}
	else {
		size = p->Write(buffer);
	}

	if (p->GetOpcode() != OP_SessionRequest && p->GetOpcode() != OP_SessionResponse) {
		if (Compressed) {
			uint32_t newlen = ProtocolPacket::Compress(buffer, size, tmpbuffer, 1024);
			memcpy(buffer, tmpbuffer, newlen);
			size = newlen + 2; // +2 for the crc
		}

		if (Encoded) {
			ProtocolPacket::ChatEncode(buffer, size - 2, Key); // -2 to remove the crc
		}

		*reinterpret_cast<uint16_t*>(buffer + size - 2) = htons(static_cast<uint16_t>(CRC16(buffer, size - 2, Key)));
	}

	if (NetDebugEnabled()) {
		DumpBytes(buffer, size);
	}
	Stream::WritePacket(Sock, buffer, size);
}

void EQ2Stream::EQ2QueuePacket(EQ2Packet* app, bool bDelete) {
	if (CheckActive()) {
		PreparePacket(app);
		SequencedPush(app);
	}
	if (bDelete) {
		delete app;
	}
}

void EQ2Stream::PreparePacket(EQ2Packet* app, uint8_t offset) {
	app->SetVersion(ClientVersion);
	CompressedOffset = 0;

	if (!app->PacketPrepared) {
		if (app->PreparePacket(MaxLength) == 255) //invalid version
			return;
	}

	if (!app->EQ2Compressed && app->Size >= 0x80) {
		CompressedOffset = EQ2_Compress(app);
		app->EQ2Compressed = true;
	}
	if (!app->PacketEncrypted) {
		EncryptPacket(app, CompressedOffset, offset);
		if (app->Size > 2 && app->buffer[2] == 0) {
			unsigned char* new_buffer = new unsigned char[app->Size + 1];
			new_buffer[2] = 0;
			memcpy(new_buffer + 3, app->buffer + 2, app->Size - 2);
			delete[] app->buffer;
			app->buffer = new_buffer;
			app->Size++;
		}
	}
}

uint8_t EQ2Stream::EQ2_Compress(EQ2Packet* app, uint8_t offset) {
	unsigned char* pDataPtr = app->buffer + offset;
	stream.next_in = pDataPtr;
	stream.avail_in = app->Size - offset;

	uint32_t outBufSize = app->Size * 2 + offset;
	unsigned char* pDataOut;
	unsigned char stackBuf[4096];
	std::unique_ptr<unsigned char[]> dynamicBuf;

	if (outBufSize <= sizeof(stackBuf)) {
		pDataOut = stackBuf;
	}
	else {
		dynamicBuf.reset(new unsigned char[outBufSize]);
		pDataOut = dynamicBuf.get();
	}

	memcpy(pDataOut, app->buffer, offset);
	stream.next_out = pDataOut + offset;
	stream.avail_out = outBufSize - offset;


	uint32_t bytes_written = 0;
	if (deflate(&stream, Z_SYNC_FLUSH) != Z_OK) {
		LogError(LOG_NET, 0, "Could not deflate packet?");
		return offset - 1;
	}

	bytes_written = outBufSize - offset - stream.avail_out;
	pDataOut[offset - 1] = 1;

	uint32_t newSize = bytes_written + offset;

	if (dynamicBuf) {	
		delete[] app->buffer;
		app->buffer = dynamicBuf.release();
	}
	else if (app->Size >= newSize) {
		memcpy(app->buffer, pDataOut, newSize);
	}
	else {
		delete[] app->buffer;
		app->buffer = new unsigned char[newSize];
		memcpy(app->buffer, pDataOut, newSize);
	}
	app->Size = newSize;

	return offset - 1;
}

void EQ2Stream::EncryptPacket(EQ2Packet* app, uint8_t compress_offset, uint8_t offset) {
	if (app->Size > 2 && crypto.isEncrypted()) {
		app->PacketEncrypted = true;
		unsigned char* crypt_buff = app->buffer;
		if (app->EQ2Compressed)
			crypto.RC4Encrypt(crypt_buff + compress_offset, app->Size - compress_offset);
		else
			crypto.RC4Encrypt(crypt_buff + 2 + offset, app->Size - 2 - offset);
	}
}

void EQ2Stream::SequencedPush(EQ2Packet* p) {
	p->SetVersion(ClientVersion);
	//Copy the basic data from the eq2 packet (buffer etc not the packet elements)
	EQ2Packet* pushCpy = p->CopyRaw();

	WriteLocker lock(seqQueueLock);
	SequencedQueue.push_back(pushCpy);
}

ProtocolPacket* EQ2Stream::SequencedPop() {
	//Check if we have any fragmented packets in the queue before trying to combine anything
	if (!fragmentedQueue.empty()) {
		ProtocolPacket* ret = fragmentedQueue.front();
		fragmentedQueue.pop_front();
		return ret;
	}

	WriteLocker lock(seqQueueLock);
	if (SequencedQueue.empty()) {
		return nullptr;
	}

	EQ2Packet* p = SequencedQueue.front();
	SequencedQueue.pop_front();

	//Try to combine this packet with other packets in the queue
	while (!SequencedQueue.empty()) {
		EQ2Packet* next = SequencedQueue.front();
		if (p->TryCombine(next, MaxLength - 6)) {
			delete next;
			SequencedQueue.pop_front();
		}
		else {
			break;
		}
	}
	lock.Unlock();

	ProtocolPacket* ret = nullptr;

	if (p->Size - 2 > MaxLength - 6) { // proto-op(2), seq(2) ... data ... crc(2)
		unsigned char* tmpbuff = p->buffer;
		uint32_t length = p->Size - 2;

		OP_Packet_Packet* out = new OP_Packet_Packet(OP_Fragment, nullptr, MaxLength);
		out->SetSequence(NextOutSeq++);
		*reinterpret_cast<uint32_t*>(out->buffer + 4) = htonl(length);
		memcpy(out->buffer + 8, tmpbuff + 2, MaxLength - 10); // 10 = op(2) + seq(2) + size(4) + crc(2)?? 

		uint32_t used = MaxLength - 10;
		uint32_t chunksize = 0;
		//We'll return this fragment now, queue the others
		ret = out;

		while (used < length) {
			chunksize = min<>(length - used, MaxLength - 6);
			out = new OP_Packet_Packet(OP_Fragment, nullptr, chunksize + 6);
			out->SetSequence(NextOutSeq++);
			memcpy(out->buffer + 4, tmpbuff + used + 2, chunksize);
			fragmentedQueue.push_back(out);
			used += chunksize;
		}
	}
	else {
		OP_Packet_Packet* out = new OP_Packet_Packet();
		out->SetSequence(NextOutSeq++);
		out->Write(p);
		ret = out;
	}

	return ret;
}

void EQ2Stream::NonSequencedPush(ProtocolPacket* p) {
	p->SetVersion(ClientVersion);
	WriteLocker lock(nonSeqQueueLock);
	NonSequencedQueue.push_back(p);
}

void EQ2Stream::Write() {
	vector<std::unique_ptr<ProtocolPacket> > ReadyToSend;
	vector<ProtocolPacket*> SeqReadyToSend;
	//This reference automatically updates
	static const uint32_t& currentTime = Timer::GetCurrentTime2();

	const int32_t decay = DecayRate;
	if (BytesWritten < decay) {
		BytesWritten = 0;
	}
	else {
		BytesWritten -= decay;
	}

	// Check our rate to make sure we can send more
	const int32_t threshold = RateThreshold;
	if (BytesWritten > threshold) {
		return;
	}

	const int32_t ack = NextAckToSend.load();
	if (ack > LastAckSent || LastAckSent == 0x0000FFFF) {
		SendAck(ack);
		if (ack > 0xFFFF) {
			NextAckToSend.fetch_sub(0xFFFF);
		}
	}

	// List of packets we will be combining
	ProtocolPacket* packetsToCombine[16];
	uint16_t numCombinePackets = 0;
	//Start off our packet size at 4 (protocol opcode + crc)
	uint16_t combinePacketSize = 4;

	auto DoPacketCombine = [this, &packetsToCombine, &numCombinePackets, &combinePacketSize]() -> ProtocolPacket* {
		//Create the newly combined packet data
		unsigned char* pbuf = new unsigned char[combinePacketSize];
		pbuf[0] = 0x00;
		pbuf[1] = OP_Combined;
		uint16_t offset = 2;

		for (uint16_t i = 0; i < numCombinePackets; i++) {
			ProtocolPacket* p = packetsToCombine[i];
			//First add the size
			uint16_t size = p->Size - 2;
			pbuf[offset++] = static_cast<uint8_t>(size);

			//Now copy the packet data
			memcpy(pbuf + offset, p->buffer, size);
			offset += size;
			delete p;
		}

		//Create the protocol packet to return
		ProtocolPacket* ret = new ProtocolPacket;
		ret->buffer = pbuf;
		ret->Size = combinePacketSize;
		ret->bBufferSet = true;
		ret->opcode = OP_Combined;
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "Combined Packet!");
			DumpBytes(ret->buffer, ret->Size);
		}
		numCombinePackets = 0;
		return ret;
	};

	auto CheckCombinePackets = [this, &packetsToCombine, &numCombinePackets, &combinePacketSize, DoPacketCombine](ProtocolPacket* p) -> ProtocolPacket* {
		ProtocolPacket* ret = nullptr;

		if (!p->buffer) {
			unsigned char* tmp = nullptr;
			p->Write(tmp);
		}

		//Strip the crc bytes off the packet we are checking
		uint32_t effectiveSize = p->Size - 2 + 1;

		if (effectiveSize > 255 || effectiveSize + combinePacketSize > MaxLength || numCombinePackets == 16) {
			ret = DoPacketCombine();
			combinePacketSize = 4;
		}

		packetsToCombine[numCombinePackets++] = p;
		combinePacketSize += effectiveSize;
		return ret;
	};


	// See if there are more non-sequenced packets left
	WriteLocker lock1(nonSeqQueueLock);
	while (!NonSequencedQueue.empty()) {
		//Check if we have a packet to combine p with...
		ProtocolPacket* nonseq = NonSequencedQueue.front();
		NonSequencedQueue.pop_front();

		if (nonseq->GetOpcode() == OP_SessionDisconnect) {
			SetState(EQStreamState::CLOSING);
			server->StreamDisconnected(shared_from_this());
		}

		//We do not want to start combining packets until our session has been established
		if (!crypto.isEncrypted()) {
			ReadyToSend.emplace_back(nonseq);
			BytesWritten += nonseq->Size;
		}
		else if (ProtocolPacket* p = CheckCombinePackets(nonseq)) {
			ReadyToSend.emplace_back(p);
			BytesWritten += p->Size;
			if (BytesWritten > threshold) {
				// Sent enough this round, lets stop to be fair
				break;
			}
		}
	}
	lock1.Unlock();

	while (BytesWritten < threshold) {
		ProtocolPacket* p = SequencedPop();

		if (!p) break;

		p->SetSentTime(currentTime);

		if (numCombinePackets == 0) {
			SeqReadyToSend.push_back(p);
			BytesWritten += p->Size;
			WriteLocker rl(resendQueueLock);
			ResendQueue.push_back(p);
			continue;
		}

		//See if we can combine this packet with our nonseq combine packet, run a quick check here instead of using the sub function
		//The sub function is allowed to delete it so we need to make a copy first if going that route

		//Factor out crc, add the size byte
		uint32_t effectiveSize = p->Size - 2 + 1;

		//Check if this is combinable
		if (effectiveSize > 255 || combinePacketSize + effectiveSize > MaxLength || numCombinePackets == 16) {
			//We cannot combine it, we need to pop the combined packet to send first
			if (numCombinePackets == 1) {
				//Send as is
				ReadyToSend.emplace_back(packetsToCombine[0]);
				BytesWritten += ReadyToSend.back()->Size;
				numCombinePackets = 0;
			}
			else {
				//Pop the combine
				ReadyToSend.emplace_back(DoPacketCombine());
				BytesWritten += ReadyToSend.back()->Size;
			}

			//Send this packet as is
			SeqReadyToSend.push_back(p);
			BytesWritten += p->Size;

			WriteLocker rl(resendQueueLock);
			ResendQueue.push_back(p);
		}
		else {
			//We can combine this packet
			//Create a copy for our resend queue and add this to our combine packet list

			//Write the opcode/sequence first
			unsigned char* tmp = nullptr;
			p->Write(tmp);

			//Now copy
			ProtocolPacket* resendCopy = p->Copy();
			resendQueueLock.WriteLock();
			ResendQueue.push_back(resendCopy);
			resendQueueLock.WriteUnlock();

			//Finally, add this to our combine packet list
			packetsToCombine[numCombinePackets++] = p;
			combinePacketSize += effectiveSize;
		}
	}

	//Check for any combine packets that need to be dealt with
	if (numCombinePackets == 1) {
		//We don't have another packet to combine with so just send this packet as is
		ReadyToSend.emplace_back(packetsToCombine[0]);
		BytesWritten += ReadyToSend.back()->Size;
	}
	else if (numCombinePackets) {
		//Do one final combine
		ReadyToSend.emplace_back(DoPacketCombine());
		BytesWritten += ReadyToSend.back()->Size;

	}

	// Send all the packets we "made"
	for (auto& packet : ReadyToSend) {
		WritePacket(packet.get());
	}

	const uint32_t ResendMSDelta = 250;
	WriteLocker lock(resendQueueLock);
	for (auto& packet : ResendQueue) {
		//Resend this packet periodically until it has been acked
		if (currentTime - packet->GetSentTime() >= ResendMSDelta) {
			WritePacket(packet);
			packet->SetSentTime(currentTime);
		}
	}
	lock.Unlock();

	for (auto& packet : SeqReadyToSend) {
		WritePacket(packet);
	}
}

void EQ2Stream::SetMaxAckReceived(uint16_t seq) {
	if (seq > MaxAckReceived || MaxAckReceived == 0xFFFF) {
		//16bit overflow
		MaxAckReceived = seq;
	}

	WriteLocker lock(resendQueueLock);
	for (auto itr = ResendQueue.begin(); itr != ResendQueue.end();) {
		ProtocolPacket* p = *itr;
		uint16_t psec = p->GetSequence();
		if (psec <= seq) {
			delete p;
			itr = ResendQueue.erase(itr);
			if (psec == 0xFFFF) {
				//16bit overflow, break the loop so we don't delete some packets on the other side
				break;
			}
		}
		else {
			break;
		}
	}
}

void EQ2Stream::AdjustRates(uint32_t average_delta) {
	if (average_delta) {
		RateThreshold = RATEBASE / average_delta;
		DecayRate = DECAYBASE / average_delta;
	}
}

uint16_t EQ2Stream::processRSAKey(ProtocolPacket* p) {
	if (p->buffer[0] == 0)
		crypto.setRC4Key(Crypto::RSADecrypt(p->buffer + 62, 8));
	else
		crypto.setRC4Key(Crypto::RSADecrypt(p->buffer + 61, 8));
	return 0;
}

bool EQ2Stream::HandleEmbeddedPacket(unsigned char* buffer, uint32_t length) {
	if (length < 3) {
		return false;
	}

	if (buffer[0] == 0 && buffer[1] == OP_AppCombined) {
		ProtocolPacket* subp = new OP_AppCombined_Packet(buffer + 2, length - 2);
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "OP_AppCombine_Packet");
			DumpBytes(subp->buffer, subp->Size);
		}
		ProcessPacket(subp);
		delete subp;
		return true;
	}
	else if (buffer[0] == 0 && buffer[1] == 0) {
		EQ2Packet* newpacket = ProcessEncryptedData(buffer + 1, length - 1);
		if (newpacket) {
			if (NetDebugEnabled()) {
				LogInfo(LOG_PACKET, 0, "Decrypted packet");
				DumpBytes(newpacket->buffer, newpacket->Size);
			}
			InboundQueuePush(newpacket);
		}
		else
			LogError(LOG_PACKET, 0, "No Packet!");
		return true;
	}

	return false;
}

EQ2Packet* EQ2Stream::ProcessEncryptedData(unsigned char* data, uint32_t size) {
	uint16_t opcode;
	if (NetDebugEnabled()) {
		LogDebug(LOG_PACKET, 0, "Decrypt Before: ");
		DumpBytes(data, size);
	}
	crypto.RC4Decrypt(data, size);
	if (NetDebugEnabled()) {
		LogDebug(LOG_PACKET, 0, "Decrypt After: ");
		DumpBytes(data, size);
	}
	uint32_t offset = 0;
	if (data[0] == 0xFF && size > 2) {
		offset = 3;
		memcpy(&opcode, data + sizeof(uint8_t), sizeof(uint16_t));
	}
	else {
		offset = 1;
		opcode = data[0];
	}

	if (ClientVersion == 0 && (opcode == 0 || opcode == 1)) {
		ReadVersionPacket(data, size, offset, opcode);
		return nullptr;
	}

	std::unique_ptr<EQ2Packet> ret(OpcodeManager::GetGlobal()->GetPacketForVersion(ClientVersion, opcode));
	if (ret) {
		if (!ret->Read(data, offset, size)) {
			if (NetDebugEnabled()) {
				DumpBytes(data + offset, size - offset);
				LogWarn(LOG_PACKET, 0, "BLAH!!!");
			}
		}

		//Check if there is a sub packet - used for packets that change structs based on the value of an element
		//One example is ClientCmdMsg
		while (std::unique_ptr<EQ2Packet> p{ ret->GetSubPacket() }) {
			if (p->GetOpcode() == ret->GetOpcode()) {
				//Got an infinite loop here but it was due to corrupted encryption, adding a break just incase
				break;
			}
			ret = std::move(p);
			if (!ret->Read(data, offset, size)) {
				if (NetDebugEnabled()) {
					DumpBytes(data + offset, size - offset);
					LogWarn(LOG_PACKET, 0, "BLAH!!!");
				}
			}
		}
	}
	else {
		LogDebug(LOG_PACKET, 0, "Unhandled opcode %u", opcode);
		DumpBytes(data + offset, size - offset);
	}

	return ret.release();
}

EQ2Packet* EQ2Stream::ProcessEncryptedPacket(ProtocolPacket* p) {
	EQ2Packet* ret = nullptr;
	if (p->GetOpcode() == OP_Packet && p->Size > 2)
		ret = ProcessEncryptedData(p->buffer + 2, p->Size - 2);
	else
		ret = ProcessEncryptedData(p->buffer, p->Size);
	return ret;
}

void EQ2Stream::InboundQueuePush(EQ2Packet* p) {
	WriteLocker lock(inboundQueueLock);
	InboundQueue.push_back(p);
}

EQ2Packet* EQ2Stream::PopPacket() {
	EQ2Packet* p = nullptr;

	WriteLocker lock(inboundQueueLock);
	if (InboundQueue.size()) {
		p = InboundQueue.front();
		InboundQueue.pop_front();
	}
	lock.Unlock();

	if (p)
		p->SetVersion(ClientVersion);
	return p;
}

void EQ2Stream::InboundQueueClear() {
	WriteLocker lock(inboundQueueLock);
	for (auto& itr : InboundQueue) {
		delete itr;
	}
	InboundQueue.clear();
}

void EQ2Stream::QueuePacket(EQ2Packet* p, bool bDelete) {
	unsigned char* buf = nullptr;
	p->Write(buf);
	if (p->bOpcodeError) {
		//The opcode manager will spit out an error about this
		if (bDelete)
			delete p;
	}
	else {
		if (NetDebugEnabled()) {
			DumpBytes(buf, p->Size);
		}
		EQ2QueuePacket(p, bDelete);
	}
}

void EQ2Stream::QueuePacket(EQ2Packet& packet) {
	QueuePacket(&packet, false);
}

void EQ2Stream::SendAck(uint16_t seq) {
	SetLastAckSent(seq);
	OP_Ack_Packet* ack = new OP_Ack_Packet();
	ack->Sequence = seq;
	NonSequencedPush(ack);
}

void EQ2Stream::SetLastAckSent(int32_t seq) {
	LastAckSent = seq;
}

void EQ2Stream::SendSessionResponse() {
	OP_SessionResponse_Packet* Response = new OP_SessionResponse_Packet();
	Response->Session = htonl(Session);
	Response->MaxLength = htonl(MaxLength);
	Response->UnknownA = 2;
	Response->Format = 0;
	Response->UnknownB = 0;
	Response->UnknownD = 0;

	if (Compressed)
		Response->Format |= FLAG_COMPRESSED;
	if (Encoded)
		Response->Format |= FLAG_ENCODED;

	Response->Key = htonl(Key);


	NonSequencedPush(Response);
}

void EQ2Stream::SendDisconnect(uint16_t reason) {
	auto disconnect = new OP_SessionDisconnect_Packet;
	disconnect->Session = htonl(Session);
	disconnect->Reason = htons(reason);

	NonSequencedPush(disconnect);

	Disconnected();
}

void EQ2Stream::SendKeepAlive() {
	OP_KeepAlive_Packet* keepAlive = new OP_KeepAlive_Packet();

	NonSequencedPush(keepAlive);
}

void EQ2Stream::SendServerSessionUpdate(uint16_t requestID) {
	OP_ServerSessionUpdate_Packet* update = new OP_ServerSessionUpdate_Packet();

	uint32_t sent = ntohl(SentPackets);
	uint32_t received = ntohl(ReceivedPackets);

	update->RequestID = requestID;
	update->CurrentTime = ntohl(Timer::GetCurrentTime2());
	update->SentPackets = sent;
	update->SentPackets2 = sent;
	update->ReceivedPackets = received;
	update->ReceivedPackets2 = received;

	NonSequencedPush(update);
}

#include "Packets/EQ2Packets/OP_KeyRequest_Packet.h"
void EQ2Stream::SendKeyRequest() {
	OP_KeyRequest_Packet* req = new OP_KeyRequest_Packet(ClientVersion);
	unsigned char* buf = nullptr;
	req->Write(buf);
	EQ2QueuePacket(req);
}

void EQ2Stream::ProcessFragmentedData(ProtocolPacket* p) {
	if (oversize_buffer) {
		memcpy(oversize_buffer + oversize_offset, p->buffer + 2, p->Size - 2);
		oversize_offset += p->Size - 2;
		if (oversize_offset == oversize_length) {
			if (oversize_buffer[0] == 0x00 && oversize_buffer[1] == 0x19) {
				ProtocolPacket* subp = new OP_AppCombined_Packet(oversize_buffer + 2, oversize_length - 2);
				ProcessPacket(subp);
				delete subp;
			}
			else {
				if (crypto.isEncrypted() && p && p->Size > 2) {
					EQ2Packet* p2 = ProcessEncryptedData(oversize_buffer, oversize_offset);
					if (p2) {
						InboundQueuePush(p2);
					}
				}
			}
			delete[] oversize_buffer;
			oversize_buffer = nullptr;
			oversize_offset = 0;
		}
		return;
	}

	//This must be the start of new fragmented data, allocate a buffer
	oversize_length = ntohl(*reinterpret_cast<uint32_t*>(p->buffer + 2));
	oversize_buffer = new unsigned char[oversize_length];
	memcpy(oversize_buffer, p->buffer + 6, p->Size - 6);
	oversize_offset = p->Size - 6;
}

const char* EQ2Stream::GetDisconnectReasonString(uint16_t reason){
	const char* result;

	switch (reason)
	{
	case 0:
		result = "DisconnectReasonNone";
		break;
	case 1:
		result = "DisconnectReasonConnectionRefused";
		break;
	case 2:
		result = "DisconnectReasonTooManyConnections";
		break;
	case 3:
		result = "DisconnectReasonApplication";
		break;
	case 4:
		result = "DisconnectReasonTimeout";
		break;
	case 5:
		result = "DisconnectReasonApplicationReleased";
		break;
	case 6:
		result = "DisconnectReasonSocketError";
		break;
	case 8:
		result = "DisconnectReasonOtherSideTerminated";
		break;
	case 9:
		result = "DisconnectReasonManagerDeleted";
		break;
	case 11:
		result = "DisconnectReasonConnectFail";
		break;
	case 10:
		result = "DisconnectReasonConnectError";
		break;
	case 7:
		result = "DisconnectReasonSocketErrorDuringNegotiation";
		break;
	case 12:
		result = "DisconnectReasonLogicalPacketTooShort";
		break;
	case 13:
		result = "DisconnectReasonLogicalPacketTooLong";
		break;
	case 14:
		result = "DisconnectReasonConnectTimeout";
		break;
	case 15:
		result = "DisconnectReasonConnectionReset";
		break;
	case 16:
		result = "DisconnectReasonConnectionAborted";
		break;
	case 17:
		result = "DisconnectReasonDnsFailure";
		break;
	case 18:
		result = "DisconnectReasonUnableToCreateSocket";
		break;
	case 19:
		result = "DisconnectReasonUnableToConfigureSocket";
		break;
	default:
		result = "UnknownReason";
		break;
	}
	return result;
}