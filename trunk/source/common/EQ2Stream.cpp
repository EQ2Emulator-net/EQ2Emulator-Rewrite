#include "EQ2Stream.h"
#include "log.h"
#include "Packets/ProtocolPackets/ProtocolPackets.h"
#include "CRC16.h"
#include "util.h"
#include "Server.h"
#include "timer.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Stream::EQ2Stream(unsigned int ip, unsigned short port) : Stream(ip, port) {
	Key = 0;
	Session = 0;
	MaxLength = 0;
	NextInSeq = 0;
	NextOutSeq = 0;
	MaxAckReceived = -1;
	NextAckToSend = -1;
	LastAckSent = -1;
	LastSeqSent = -1;
	Compressed = false;
	Encoded = false;
	CombinedAppPacket = nullptr;

	RateThreshold = RATEBASE / 250;
	DecayRate = DECAYBASE / 250;
	BytesWritten = 0;
	crypto.setRC4Key(0);
}

void EQ2Stream::Process(unsigned char* data, unsigned int length) {
	Stream::Process(data, length);
	// TODO: Validate crc and decompress or decode

	ProtocolPacket* p = ProtocolPacket::GetProtocolPacket(data, length);
	if (p) {
		ProcessPacket(p);
		delete p;
	}
}

void EQ2Stream::ProcessPacket(ProtocolPacket* p) {
	LogDebug(LOG_NET, 0, "ProtocolPacket Received, opcode: %u", p->GetOpcode());

	switch (p->GetOpcode()) {
	case OP_SessionRequest: {
		OP_SessionRequest_Packet* request = (OP_SessionRequest_Packet*)p;
		
		Session = ntohl(request->Session);
		MaxLength = ntohl(request->MaxLength);
		NextInSeq = 0;
		Key = 0x33624702;
		LogDebug(LOG_NET, 0, "OP_SessionRequest unknowna: %u, Session: %u, MaxLength: %u", request->UnknownA, request->Session, request->MaxLength);

		SendSessionResponse();
		SetState(ESTABLISHED);
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
		//AdjustRates(ntohl(Stats->average_delta));
		SendServerSessionUpdate(update->RequestID);
		if(!crypto.isEncrypted())
			SendKeyRequest();
		
		break;
	}
	case OP_Ack: {
		OP_Ack_Packet* ack = (OP_Ack_Packet*)p;
		SetMaxAckReceived(ack->Sequence);
		break;
	}
	default:
		break;
	}
}

bool EQ2Stream::HandlePacket() {
	LogError(LOG_NET, 0, "HandlePacket should be overriden");
	return false;
}

bool EQ2Stream::ValidateCRC(unsigned char* buffer, uint16_t length, uint32_t key) {
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
void EQ2Stream::EQ2QueuePacket(EQ2Packet* app, bool attempted_combine) {
	if (CheckActive()) {
		if (app->Size < 600 && !attempted_combine) {
			//MCombineQueueLock.lock();
			combine_queue.push_back(app);
			//MCombineQueueLock.unlock();
		}
		else {
			PreparePacket(app);
#ifdef LE_DEBUG
			LogWrite(PACKET__DEBUG, 0, "Packet", "After B in %s, line %i:", __FUNCTION__, __LINE__);
			DumpPacket(app);
#endif
			SendPacket(app);
		}
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

#ifdef LE_DEBUG
	LogWrite(PACKET__DEBUG, 0, "Packet", "Before Compress in %s, line %i:", __FUNCTION__, __LINE__);
	DumpPacket(app);
#endif

	unsigned char* pDataPtr = app->buffer + offset;
	unsigned char* deflate_buff = new unsigned char[app->Size];
	//MCompressData.lock();
	stream.next_in = pDataPtr;
	stream.avail_in = app->Size - offset;
	stream.next_out = deflate_buff;
	stream.avail_out = app->Size;

	deflate(&stream, Z_SYNC_FLUSH);
	uint32_t newsize = app->Size - stream.avail_out;
	if (app->buffer)
		delete[] app->buffer;

	app->Size = newsize + offset;
	app->buffer = new unsigned char[app->Size];
	app->buffer[(offset - 1)] = 1;
	memcpy(app->buffer + offset, deflate_buff, newsize);
	//MCompressData.unlock();
	if (deflate_buff)
		delete[] deflate_buff;

#ifdef LE_DEBUG
	LogWrite(PACKET__DEBUG, 0, "Packet", "After Compress in %s, line %i:", __FUNCTION__, __LINE__);
	DumpPacket(app);
#endif

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

void EQ2Stream::SendPacket(EQ2Packet* p) {
	//uint32_t chunksize, used;
	//uint32_t length;

	// Convert the EQApplicationPacket to 1 or more EQProtocolPackets
	/*if (p->Size > (MaxLength - 8)) { // proto-op(2), seq(2), app-op(2) ... data ... crc(2)
		unsigned char *tmpbuff = new unsigned char[p->Size + 2];

		length = p->serialize(tmpbuff);

		ProtocolPacket *out = new ProtocolPacket(OP_Fragment, NULL, MaxLength - 4);
		*(uint32_t *)(out->buffer + 2) = htonl(p->Size);
		memcpy(out->buffer + 6, tmpbuff, MaxLength - 10);
		used = MaxLength - 10;
		SequencedPush(out);
		while (used < length) {
			out = new ProtocolPacket(OP_Fragment, NULL, MaxLength - 4);
			chunksize = min(length - used, MaxLength - 6);
			memcpy(out->buffer + 2, tmpbuff + used, chunksize);
			out->Size = chunksize + 2;
			SequencedPush(out);
			used += chunksize;
		}
		delete p;
		delete[] tmpbuff;
	}
	else {*/
	OP_Packet_Packet* out = new OP_Packet_Packet();
	out->Write(p);
		//ProtocolPacket *out = new ProtocolPacket(OP_Packet, NULL, p->Size + 2);
		//p->serialize(out->buffer + 2);
		SequencedPush(out);
		//cerr << "2: Deleting 0x" << hex << (uint32)(p) << dec << endl;
		delete p;
	/*}*/
}

void EQ2Stream::SequencedPush(ProtocolPacket *p) {
	p->SetVersion(ClientVersion);
	//MOutboundQueue.lock();
	*(uint16_t *)(p->buffer) = htons(NextOutSeq);
	p->SetSequence(NextOutSeq);
	SequencedQueue.push_back(p);
	NextOutSeq++;
	//MOutboundQueue.unlock();
}

void EQ2Stream::NonSequencedPush(ProtocolPacket *p) {
	p->SetVersion(ClientVersion);
	//MOutboundQueue.lock();
	NonSequencedQueue.push_back(p);
	//MOutboundQueue.unlock();
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
	CombinedAppPacket = NULL;
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

	//The non-seq loop must have broke before we sent this packet, send it now
	if (p) {
		ReadyToSend.push_back(p);
		BytesWritten += p->Size;
	}

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

	// Unlock the queue
	//MOutboundQueue.unlock();

	// Send all the packets we "made"
	while (ReadyToSend.size()) {
		WritePacket(ReadyToSend.front());
		delete ReadyToSend.front();
		ReadyToSend.pop_front();
	}

	while (SeqReadyToSend.size()) {
		WritePacket(SeqReadyToSend.front());
		SeqReadyToSend.pop_front();
	}
}

void EQ2Stream::SetMaxAckReceived(uint32_t seq) {
	deque<ProtocolPacket *>::iterator itr;

	//MAcks.lock();
	MaxAckReceived = seq;
	//MAcks.unlock();
	//MOutboundQueue.lock();
	if (long(seq) > LastSeqSent)
		LastSeqSent = seq;
	//MResendQue.lock();
	ProtocolPacket* packet = 0;
	for (itr = ResendQueue.begin(); itr != ResendQueue.end(); itr++) {
		packet = *itr;
		if (packet && packet->GetSequence() <= seq) {
			delete packet;
			itr = ResendQueue.erase(itr);
			if (itr == ResendQueue.end())
				break;
		}
	}
	//MResendQue.unlock();
	//MOutboundQueue.unlock();
}

void EQ2Stream::SetLastAckSent(int32_t seq) {
	//MAcks.lock();
	LastAckSent = seq;
	//MAcks.unlock();
}

void EQ2Stream::SendAck(uint16_t seq) {
	uint16_t Seq = htons(seq);
	SetLastAckSent(seq);
	OP_Ack_Packet ack;
	ack.Sequence = seq;
	NonSequencedPush(&ack);
}

void EQ2Stream::SendSessionResponse() {
	OP_SessionResponse_Packet* Response = new OP_SessionResponse_Packet();;
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
	server->StreamDisconnected(this); // this deletes the stream (client)
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
	OP_KeyRequest_Packet* req = new OP_KeyRequest_Packet();
	unsigned char* buf = nullptr;
	req->Write(buf);
	EQ2QueuePacket(req, true);
}