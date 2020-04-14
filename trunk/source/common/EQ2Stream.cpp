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
	NextAddSeq = 0;
	MaxAckReceived = -1;
	NextAckToSend = -1;
	LastAckSent = -1;
	LastSeqSent = -1;
	Compressed = false;
	Encoded = false;
	bNeedNewClient = false;
	CombinedAppPacket = nullptr;
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
}

EQ2Stream::~EQ2Stream() {

	if (CombinedAppPacket)
		delete CombinedAppPacket;

	while (NonSequencedQueue.size()) {
		delete NonSequencedQueue.front();
		NonSequencedQueue.pop_front();
	}

	while (SequencedQueue.size()) {
		delete SequencedQueue.front();
		SequencedQueue.pop_front();
	}
	
	while (ResendQueue.size()) {
		delete ResendQueue.front();
		ResendQueue.pop_front();
	}

	InboundQueueClear();

	deflateEnd(&stream);
}

void EQ2Stream::Process(const unsigned char* data, unsigned int length) {
	Stream::Process(data, length);
	
	if (!ValidateCRC(data, length, Key)) {
		LogError(LOG_NET, 0, "Packet CRC check failed. Client %s", ToString().c_str());
		return;
	}

	ProtocolPacket* p = ProtocolPacket::GetProtocolPacket(data, length, true);
	if (p) {
		ProcessPacket(p);
		delete p;
		ProcessFuturePacketQueue();
	}
}

void EQ2Stream::ProcessFuturePacketQueue() {
	if (FuturePackets.empty()) {
		return;
	}

	for (;;) {
		auto itr = FuturePackets.find(NextInSeq);
		if (itr == FuturePackets.end()) {
			break;
		}

		ProcessPacket(itr->second.get());
		FuturePackets.erase(itr);
	}
}

void EQ2Stream::ProcessPacket(ProtocolPacket* p) {
	LogDebug(LOG_NET, 0, "ProtocolPacket Received, opcode: %u", p->GetOpcode());

	switch (p->GetOpcode()) {
	case OP_SessionRequest: {
		OP_SessionRequest_Packet* request = (OP_SessionRequest_Packet*)p;

		if (Session != 0) {
			bNeedNewClient = true;
			return;
		}

		Session = ntohl(request->Session);
		MaxLength = ntohl(request->MaxLength);
		NextInSeq = 0;
		Key = 0x33624702;
		LogDebug(LOG_NET, 0, "OP_SessionRequest unknowna: %u, Session: %u, MaxLength: %u", request->UnknownA, request->Session, request->MaxLength);

		SendSessionResponse();
		SetState(EQStreamState::ESTABLISHED);
		break;
	}
	case OP_SessionDisconnect: {
		OP_SessionDisconnect_Packet* disconnect = (OP_SessionDisconnect_Packet*)p;
		SendDisconnect(disconnect->Reason);
		break;
	}
	case OP_KeepAlive: {
		SendKeepAlive();
		break;
	}
	case OP_ClientSessionUpdate: {
		OP_ClientSessionUpdate_Packet* update = (OP_ClientSessionUpdate_Packet*)p;
		AdjustRates(ntohl(update->AverageDelta));
		SendServerSessionUpdate(update->RequestID);
		if(!crypto.isEncrypted())
			SendKeyRequest();
		
		break;
	}
	case OP_Packet: {
		LogDebug(LOG_PACKET, 0, "OP_Packet_Packet Dump");
		OP_Packet_Packet* pp = (OP_Packet_Packet*)p;
		DumpBytes(pp->buffer, pp->Size);
		uint16_t seq = ntohs(*(uint16_t*)(pp->buffer));
		int8_t check = CompareSequence(NextInSeq, seq);

		LogDebug(LOG_PACKET, 0, "seq = %u, NextInSeq = %u, check = %i", seq, NextInSeq, check);
		if (check > 0) {
			// Future
			LogDebug(LOG_PACKET, 0, "Future packet");

			FuturePackets[seq].reset(pp->MoveCopy());
		}
		else if (check < 0) {
			// Past
			LogDebug(LOG_PACKET, 0, "Past packet");

			NonSequencedPush(new OP_OutOfOrderAck_Packet(seq));
		}
		else {
			SetNextAckToSend(seq);
			NextInSeq++;

			if (HandleEmbeddedPacket(pp))
				break;
			if (crypto.getRC4Key() == 0 && pp && pp->Size >= 70) {
				processRSAKey(pp);
			}
			else if (crypto.isEncrypted() && pp) {
				EQ2Packet* newpacket = ProcessEncryptedPacket(p);
				if (newpacket) {
					InboundQueuePush(newpacket);
				}
			}
		}
		break;
	}
	case OP_Fragment: {
		uint16_t seq = ntohs(*(uint16_t*)(p->buffer));
		int8_t check = CompareSequence(NextInSeq, seq);

		LogDebug(LOG_PACKET, 0, "seq = %u, NextInSeq = %u, check = %i", seq, NextInSeq, check);
		if (check > 0) {
			// Future
			LogDebug(LOG_PACKET, 0, "Future packet");

			FuturePackets[seq].reset(p->MoveCopy());
		}
		else if (check < 0) {
			// Past
			LogDebug(LOG_PACKET, 0, "Past packet");

			NonSequencedPush(new OP_OutOfOrderAck_Packet(seq));
		}
		else {
			SetNextAckToSend(seq);
			NextInSeq++;

			if (oversize_buffer) {
				memcpy(oversize_buffer + oversize_offset, p->buffer + 2, p->Size - 2);
				oversize_offset += p->Size - 2;
				//cout << "Oversized is " << oversize_offset << "/" << oversize_length << " (" << (p->size-2) << ") Seq=" << seq << endl;
				if (oversize_offset == oversize_length) {
					if (oversize_buffer[0] == 0x00 && oversize_buffer[1] == 0x19) {
						ProtocolPacket* subp = new OP_AppCombined_Packet(oversize_buffer + 2, oversize_length - 2);
						ProcessPacket(subp);
						delete subp;
					}
					else {
						if (crypto.isEncrypted() && p && p->Size > 2) {
							EQ2Packet* p2 = ProcessEncryptedData(oversize_buffer, oversize_offset, p->GetOpcode());
							if (p2) {
								InboundQueuePush(p2);
							}
						}
					}
					delete[] oversize_buffer;
					oversize_buffer = NULL;
					oversize_offset = 0;
				}
			}
			else if (!oversize_buffer) {
				oversize_length = ntohl(*(uint32_t *)(p->buffer + 2));
				oversize_buffer = new unsigned char[oversize_length];
				memcpy(oversize_buffer, p->buffer + 6, p->Size - 6);
				oversize_offset = p->Size - 6;
			}
		}
		break;
	}
	case OP_Ack: {
		OP_Ack_Packet* ack = (OP_Ack_Packet*)p;
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
				subpacket_length = ntohs(*(uint16_t *)(p->buffer + processed + 1));
				offset = 3;
			}
			else
				offset = 1;

			if (crypto.getRC4Key() == 0 && p->Size >= 70) {
				processRSAKey(p);
			}
			else if (crypto.isEncrypted()) {
				if (processed + offset + subpacket_length > p->Size) {
					LogError(LOG_PACKET, 0, "An OP_AppCombined packet tried to read past the end of its buffer!");
					//DumpBytes(p->buffer, p->Size);
					return;
				}
				if (!HandleEmbeddedPacket(p, processed + offset, subpacket_length)) {
					newpacket = ProcessEncryptedData(p->buffer + processed + offset, subpacket_length, 0x19);// (uint16_t)OP_AppCombined);
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
		uint32_t offset;

		while (processed < p->Size) {
			if ((subpacket_length = *(p->buffer + processed) ) == 0xFF) {
				subpacket_length = *reinterpret_cast<uint16_t*>(p->buffer + processed + 1);
				offset = 3;
			}
			else {
				offset = 1;
			}

			if (subpacket_length == 0) {
				LogError(LOG_PACKET, 0, "Received a bad combine packet from a client! (size == 0)");
				break;
			}

			if (subpacket_length + processed + offset > p->Size) {
				LogError(LOG_PACKET, 0, "Received a bad combine packet from a client! (size > remaining_bytes)");
				break;
			}

			ProtocolPacket* subPacket = ProtocolPacket::GetProtocolPacket(p->buffer + processed + offset, subpacket_length, false);

			if (!subPacket) {
				break;
			}

			ProcessPacket(subPacket);
			delete subPacket;

			processed += offset + subpacket_length;
		}
	}
	case OP_OutOfOrderAck: {
		auto oop = static_cast<OP_OutOfOrderAck_Packet*>(p);
		uint16_t seq = oop->Sequence;
		WriteLocker lock(NextOutSeqLock);
		if (seq > MaxAckReceived && seq < NextOutSeq) {
			NextOutSeq = seq;
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
	unsigned char* buffer = nullptr;
	unsigned char tmpbuffer[1024];
	uint32_t size = p->Write(buffer);
	if (p->GetOpcode() != OP_SessionRequest && p->GetOpcode() != OP_SessionResponse) {
		if (Compressed) {
			uint32_t newlen = ProtocolPacket::Compress(buffer, size, tmpbuffer, 1024);
			memcpy(buffer, tmpbuffer, newlen);
			size = newlen + 2; // +2 for the crc
		}

		if (Encoded) {
			ProtocolPacket::ChatEncode(buffer, size - 2, Key); // -2 to remove the crc
		}

		*(uint16_t*)(buffer + (size - 2)) = htons((uint16_t)CRC16(buffer, size - 2, Key));
	}

	// The dump is for debugging, remove when this all works
	DumpBytes(buffer, size);
	Stream::WritePacket(server->GetSocket(), buffer, size);
}

// Copy & paste from old code
void EQ2Stream::EQ2QueuePacket(EQ2Packet* app, bool attempted_combine, bool bDelete) {
	if (CheckActive()) {
		//Need to implement this before uncommenting
		//if (app->Size < 600 && !attempted_combine) {
			//MCombineQueueLock.lock();
			//combine_queue.push_back(app);
			//MCombineQueueLock.unlock();
		//}
		//else {
			PreparePacket(app);
#ifdef LE_DEBUG
			LogWrite(PACKET__DEBUG, 0, "Packet", "After B in %s, line %i:", __FUNCTION__, __LINE__);
			DumpPacket(app);
#endif
			SendPacket(app, bDelete);
		//}
	}
	else if (bDelete) {
		delete app;
	}
}

void EQ2Stream::PreparePacket(EQ2Packet* app, uint8_t offset) {
	app->SetVersion(ClientVersion);
	CompressedOffset = 0;

#ifdef LE_DEBUG
	LogWrite(PACKET__DEBUG, 0, "Packet", "Before A in %s, line %i:", __FUNCTION__, __LINE__);
	DumpPacket(app);
#endif

	if (!app->PacketPrepared) {
		if (app->PreparePacket(MaxLength) == 255) //invalid version
			return;
	}

#ifdef LE_DEBUG
	LogWrite(PACKET__DEBUG, 0, "Packet", "After Prepare in %s, line %i:", __FUNCTION__, __LINE__);
	DumpPacket(app);
#endif

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

#ifdef LE_DEBUG
	LogWrite(PACKET__DEBUG, 0, "Packet", "After A in %s, line %i:", __FUNCTION__, __LINE__);
	DumpPacket(app);
#endif

}

uint8_t EQ2Stream::EQ2_Compress(EQ2Packet* app, uint8_t offset) {
	unsigned char* pDataPtr = app->buffer + offset;
	Bytef deflate_buff[4096];
	stream.next_in = pDataPtr;
	stream.avail_in = app->Size - offset;

	int32_t total_bytes_written = 0;

	for (;;) {
		stream.next_out = deflate_buff;
		stream.avail_out = sizeof(deflate_buff);
		if (deflate(&stream, Z_SYNC_FLUSH) != Z_OK) {
			break;
		}

		int32_t bytes_written = sizeof(deflate_buff) - stream.avail_out;
		memcpy(pDataPtr, deflate_buff, bytes_written);
		pDataPtr += bytes_written;
		total_bytes_written += bytes_written;

		unsigned int bytes_remaining = 0;
		if (stream.avail_in == 0 && (deflatePending(&stream, &bytes_remaining, Z_NULL), bytes_remaining == 0)) {
			break;
		}
	}

	app->Size = total_bytes_written + offset;
	app->buffer[offset - 1] = 1;

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

void EQ2Stream::SendPacket(EQ2Packet* p, bool bDelete) {
	if (p->Size > (MaxLength - 6)) { // proto-op(2), seq(2) ... data ... crc(2)
		unsigned char* tmpbuff = p->buffer;
		uint32_t length = p->Size - 2;

		OP_Packet_Packet *out = new OP_Packet_Packet(OP_Fragment, NULL, MaxLength);
		*(uint32_t *)(out->buffer + 4) = htonl(length);
		memcpy(out->buffer + 8, tmpbuff + 2, MaxLength - 10); // 10 = op(2) + seq(2) + size(4) + crc(2)?? 

		uint32_t used = MaxLength - 10;
		uint32_t chunksize = 0;
		SequencedPush(out);

		while (used < length) {
			chunksize = min(length - used, MaxLength - 6);
			out = new OP_Packet_Packet(OP_Fragment, NULL, chunksize + 6);
			memcpy(out->buffer + 4, tmpbuff + used + 2, chunksize);
			SequencedPush(out);
			used += chunksize;
		}
		if (bDelete)
			delete p;
	}
	else {
		OP_Packet_Packet* out = new OP_Packet_Packet();
		out->Write(p);
		SequencedPush(out);
		if (bDelete)
			delete p;
	}
}

void EQ2Stream::SequencedPush(ProtocolPacket *p) {
	p->SetVersion(ClientVersion);
	WriteLocker lock(seqQueueLock);	
	*(uint16_t *)(p->buffer) = htons(NextAddSeq);
	p->SetSequence(NextAddSeq++);
	SequencedQueue.push_back(p);
}

void EQ2Stream::NonSequencedPush(ProtocolPacket *p) {
	p->SetVersion(ClientVersion);
	WriteLocker lock(nonSeqQueueLock);
	NonSequencedQueue.push_back(p);
}

void EQ2Stream::Write() {
	deque<ProtocolPacket*> ReadyToSend;
	deque<ProtocolPacket*> SeqReadyToSend;
	long maxack;
	map<uint16_t, ProtocolPacket *>::iterator sitr;

	// Check our rate to make sure we can send more
	//MRate.lock();
	int32_t threshold = RateThreshold;
	//MRate.unlock();
	if (BytesWritten > threshold) {
		//cout << "Over threshold: " << BytesWritten << " > " << threshold << endl;
		return;
	}

	//MCombinedAppPacket.lock();
	EQ2Packet *CombPack = CombinedAppPacket;
	CombinedAppPacket = nullptr;
	//MCombinedAppPacket.unlock();

	if (CombPack) {
		SendPacket(CombPack);
	}

	// If we got more packets to we need to ack, send an ack on the highest one
	//MAcks.lock();
	maxack = MaxAckReceived;
	// Added from peaks findings
	if (NextAckToSend > LastAckSent || LastAckSent == 0x0000ffff)
		SendAck(NextAckToSend);
	//MAcks.unlock();

	// Lock the outbound queues while we process
	//MOutboundQueue.lock();

	// Adjust where we start sending in case we get a late ack
	if (maxack > LastSeqSent)
		LastSeqSent = maxack;

	// Place to hold the base packet t combine into
	ProtocolPacket *p = NULL;

	// Loop until both are empty or MaxSends is reached

	// See if there are more non-sequenced packets left
	WriteLocker lock1(nonSeqQueueLock);
	while (NonSequencedQueue.size()) {
		if (!p) {
			// If we don't have a packet to try to combine into, use this one as the base
			// And remove it form the queue
			p = NonSequencedQueue.front();
			NonSequencedQueue.pop_front();
		}
		//Check if we have a packet to combine p with...
		if (NonSequencedQueue.size()) {
			if (/*!p->combine(NonSequencedQueue.front())*/ true) {
				// Tryint to combine this packet with the base didn't work (too big maybe)
				// So just send the base packet (we'll try this packet again later)
				ReadyToSend.push_back(p);
				BytesWritten += p->Size;
				p = NULL;
			}
			else {
				// Combine worked, so just remove this packet and it's spot in the queue
				delete NonSequencedQueue.front();
				NonSequencedQueue.pop_front();
			}
		}
		else {
			//We have no packets to combine p with so just send it...
			ReadyToSend.push_back(p);
			BytesWritten += p->Size;
			p = NULL;
		}
		if (BytesWritten > threshold) {
			// Sent enough this round, lets stop to be fair
			break;
		}
	}
	lock1.Unlock();

	//The non-seq loop must have broke before we sent this packet, send it now
	if (p) {
		ReadyToSend.push_back(p);
		BytesWritten += p->Size;
	}

	WriteLocker lock2(seqQueueLock);
	if (SequencedQueue.size() && BytesWritten < threshold) {
		while (SequencedQueue.size()) {
			p = SequencedQueue.front();
			BytesWritten += p->Size;
			SeqReadyToSend.push_back(p);
			p->SetSentTime(Timer::GetCurrentTime2());
			ResendQueue.push_back(p);
			SequencedQueue.pop_front();
			LastSeqSent = p->GetSequence();
			if (BytesWritten > threshold) {
				break;
			}
		}
	}
	lock2.Unlock();

	// Unlock the queue
	//MOutboundQueue.unlock();

	// Send all the packets we "made"
	while (ReadyToSend.size()) {
		WritePacket(ReadyToSend.front());
		delete ReadyToSend.front();
		ReadyToSend.pop_front();
	}

	WriteLocker l(NextOutSeqLock);
	if (NextOutSeq < LastSeqSent) {
		WriteLocker lock(resendQueueLock);
		for (auto& itr : ResendQueue) {
			WritePacket(itr);
		}
	}

	while (SeqReadyToSend.size()) {
		WritePacket(SeqReadyToSend.front());
		++NextOutSeq;
		++LastSeqSent;
		SeqReadyToSend.pop_front();
	}
}

void EQ2Stream::SetMaxAckReceived(int32_t seq) {
	deque<ProtocolPacket *>::iterator itr;

	MaxAckReceived = seq;

	if (seq > LastSeqSent)
		LastSeqSent = seq;
	
	WriteLocker lock(resendQueueLock);
	for (auto itr = ResendQueue.begin(); itr != ResendQueue.end();) {
		ProtocolPacket* p = *itr;
		if (p->GetSequence() <= seq) {
			delete p;
			itr = ResendQueue.erase(itr);
		}
		else {
			break;
		}
	}
}

void EQ2Stream::SetLastAckSent(int32_t seq) {
	//MAcks.lock();
	LastAckSent = seq;
	//MAcks.unlock();
}

void EQ2Stream::AdjustRates(uint32_t average_delta) {
	if (average_delta) {
		//MRate.lock();
		RateThreshold = RATEBASE / average_delta;
		DecayRate = DECAYBASE / average_delta;
		//MRate.unlock();
	}
}

int8_t EQ2Stream::CompareSequence(uint16_t expected_seq, uint16_t seq) {
	if (expected_seq == seq) {
		// Curent
		return 0;
	}
	else if ((seq > expected_seq/* && (uint32_t)seq < ((uint32_t)expected_seq + EQ2Stream::MaxWindowSize)) || seq < (expected_seq - EQ2Stream::MaxWindowSize*/)) { // No clue what this MaxWindowSize is for, set to 2048 in old code
		// Future
		return 1;
	}
	else {
		// Past
		return -1;
	}
}

void EQ2Stream::SetNextAckToSend(int32_t seq) {
	//MAcks.lock();
	NextAckToSend = seq;
	//MAcks.unlock();
}

uint16_t EQ2Stream::processRSAKey(ProtocolPacket *p) {
	if (p->buffer[0] == 0)
		crypto.setRC4Key(Crypto::RSADecrypt(p->buffer + 62, 8));
	else
		crypto.setRC4Key(Crypto::RSADecrypt(p->buffer + 61, 8));
	return 0;
}

bool EQ2Stream::HandleEmbeddedPacket(ProtocolPacket* p, uint16_t offset, uint16_t length) {
	if (p && p->Size >= ((uint32_t)(offset + 2))) {
		if (p->buffer[offset] == 0 && p->buffer[offset + 1] == 0x19) {
			if (length == 0)
				length = p->Size - 2 - offset;
			else
				length -= 2;
#ifdef LE_DEBUG
			LogWrite(PACKET__DEBUG, 0, "Packet", "Creating OP_AppCombined Packet!");
#endif
			ProtocolPacket* subp = new OP_AppCombined_Packet(p->buffer + 2 + offset, length);
			LogDebug(LOG_PACKET, 0, "OP_AppCombine_Packet");
			DumpBytes(subp->buffer, subp->Size);
			//subp->copyInfo(p);
			ProcessPacket(subp);
			delete subp;
			return true;
		}
		else if (p->buffer[offset] == 0 && p->buffer[offset + 1] == 0) {
			if (length == 0)
				length = p->Size - 1 - offset;
			else
				length--;
#ifdef LE_DEBUG
			LogWrite(PACKET__DEBUG, 0, "Packet", "Creating Opcode 0 Packet!");
			DumpPacket(p->pBuffer + 1 + offset, length);
#endif
			EQ2Packet* newpacket = ProcessEncryptedData(p->buffer + 1 + offset, length, OP_Packet);
			if (newpacket) {
				LogInfo(LOG_PACKET, 0, "Decrypted packet");
				DumpBytes(newpacket->buffer, newpacket->Size);
				InboundQueuePush(newpacket);
			}
			else
				LogError(LOG_PACKET, 0, "No Packet!");
			return true;
		}
	}
	return false;
}

EQ2Packet* EQ2Stream::ProcessEncryptedData(unsigned char* data, uint32_t size, uint16_t opcode) {
	crypto.RC4Decrypt(data, size);
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
			DumpBytes(data + offset, size - offset);
			LogWarn(LOG_PACKET, 0, "BLAH!!!");
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
				DumpBytes(data + offset, size - offset);
				LogWarn(LOG_PACKET, 0, "BLAH!!!");
			}
		}
	}
	else {
		LogDebug(LOG_PACKET, 0, "Unhandled opcode %u", opcode);
		DumpBytes(data + offset, size - offset);
	}

	return ret.release();
}

EQ2Packet* EQ2Stream::ProcessEncryptedPacket(ProtocolPacket *p) {
	EQ2Packet* ret = NULL;
	if (p->GetOpcode() == OP_Packet && p->Size > 2)
		ret = ProcessEncryptedData(p->buffer + 2, p->Size - 2, p->GetOpcode());
	else
		ret = ProcessEncryptedData(p->buffer, p->Size, p->GetOpcode());
	return ret;
}

void EQ2Stream::InboundQueuePush(EQ2Packet* p) {
	WriteLocker lock(inboundQueueLock);
	InboundQueue.push_back(p);
}

EQ2Packet* EQ2Stream::PopPacket() {
	EQ2Packet *p = NULL;

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
	while (InboundQueue.size()) {
		delete InboundQueue.front();
		InboundQueue.pop_front();
	}
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
		DumpBytes(buf, p->Size);
		EQ2QueuePacket(p, true, bDelete);
	}
}

void EQ2Stream::QueuePacket(EQ2Packet& packet) {
	QueuePacket(&packet, false);
}

void EQ2Stream::SendAck(uint16_t seq) {
	SetLastAckSent(seq);
	OP_Ack_Packet* ack = new OP_Ack_Packet();
	ack->Sequence = htons(seq);
	NonSequencedPush(ack);
}

void EQ2Stream::SendSessionResponse() {
	OP_SessionResponse_Packet* Response = new OP_SessionResponse_Packet();
	Response->Session = htonl(Session);
	Response->MaxLength = htonl(MaxLength);
	Response->UnknownA = 2;
	Response->Format = 0;

	if (Compressed)
		Response->Format |= FLAG_COMPRESSED;
	if (Encoded)
		Response->Format |= FLAG_ENCODED;

	Response->Key = htonl(Key);


	NonSequencedPush(Response);
}

void EQ2Stream::SendDisconnect(uint16_t reason) {
	OP_SessionDisconnect_Packet disconnect;
	disconnect.Session = htonl(Session);
	disconnect.Reason = reason;

	// Send this now and kill the client.
	WritePacket(&disconnect);
	server->StreamDisconnected(shared_from_this()); // this deletes the stream (client)
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
	update->CurrentTime = Timer::GetCurrentTime2();
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
	EQ2QueuePacket(req, true);
}