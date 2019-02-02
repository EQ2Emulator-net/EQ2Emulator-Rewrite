#include "ProtocolPacket.h"
#include "ProtocolPackets/ProtocolPackets.h"
#include "../util.h"
#include "EQ2Packet.h"
#include "../Crypto.h"
#include "../CRC16.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

ProtocolPacket::ProtocolPacket() {
	HasCRC = false;
}

uint32_t ProtocolPacket::CalculateSize() {
	return Packet::CalculateSize() + (HasCRC ? 4 : 2);
}

uint32_t ProtocolPacket::Write(unsigned char* writeBuffer) {	
	uint32_t offset = WriteOpcode(writeBuffer);
	offset += Packet::Write(writeBuffer + offset);
	return offset;
}

uint32_t ProtocolPacket::WriteOpcode(unsigned char* writeBuffer) {
	uint16_t op = htons(opcode);
	memcpy(writeBuffer, &op, 2);
	return 2;
}

ProtocolPacket* ProtocolPacket::GetProtocolPacket(const unsigned char* in_buff, uint32_t len, uint16_t version, Crypto& crypto) {
	ProtocolPacket* ret = nullptr;
	uint16_t opcode = ntohs(*(uint16_t*)in_buff);
	uint32_t offset = 2;

	if (crypto.getRC4Key() == 0 && opcode == OP_Packet && len >= 70) {
		crypto.ReadRSAKey(in_buff + offset);
	}

	switch (opcode) {
	case OP_SessionRequest: {
		ret = new OP_SessionRequest_Packet();
		break;
	}
	case OP_SessionDisconnect: {
		ret = new OP_SessionDisconnect_Packet();
		break;
	}
	case OP_KeepAlive: {
		ret = new OP_KeepAlive_Packet();
		break;
	}
	case OP_ClientSessionUpdate: {
		ret = new OP_ClientSessionUpdate_Packet();
		break;
	}
	case OP_Packet: {
		DumpBytes(in_buff + offset, len - offset - 2);
		ret = new OP_Packet_Packet(in_buff + offset, len - offset - 2, version); // -2 to trim the crc
		break;
	}
	case OP_Ack: {
		ret = new OP_Ack_Packet();
		break;
	}
	default: {
		LogError(LOG_PACKET, 0, "Unknown protocol packet opcode %u", opcode);
		break;
	}
	}

	if (ret && opcode != OP_Packet)
		ret->Read(in_buff, offset, len);

	return ret;
}

// Copy and paste from old code
//EDIT theFoof changed from static function to just use internal buffer
uint32_t ProtocolPacket::Compress() {
	unsigned char newbuf[1024];
	uint32_t newbufsize = sizeof(newbuf);
	uint32_t length = size;

	//Our buffer has 2 bytes extra on it, we don't want to compress that
	if (HasCRC) {
		length -= 2;
	}

	uint32_t flag_offset = 1, newlength;
	newbuf[0] = buffer[0];
	if (buffer[0] == 0) {
		flag_offset = 2;
		newbuf[1] = buffer[1];
	}
	if (length > 30) {
		newlength = Deflate(buffer + flag_offset, length - flag_offset, newbuf + flag_offset + 1, newbufsize);
		*(newbuf + flag_offset) = 0x5a;
		newlength += flag_offset + 1;
	}
	else {
		memmove(newbuf + flag_offset + 1, buffer + flag_offset, length - flag_offset);
		*(newbuf + flag_offset) = 0xa5;
		newlength = length + 1;
	}

	//Add our 2 crc bytes back in
	if (HasCRC) {
		newlength += 2;
	}

	if (size < newlength) {
		delete[] buffer;
		buffer = new unsigned char[newlength];
	}

	memcpy(buffer, newbuf, newlength);
	size = newlength;

	return newlength;
}

void ProtocolPacket::ChatDecode(unsigned char *buffer, int size, int DecodeKey) {
	if (buffer[1] != 0x01 && buffer[0] != 0x02 && buffer[0] != 0x1d) {
		int Key = DecodeKey;
		unsigned char *test = (unsigned char *)malloc(size);
		buffer += 2;
		size -= 2;

		int i;
		for (i = 0; i + 4 <= size; i += 4) {
			int pt = (*(int*)&buffer[i]) ^ (Key);
			Key = (*(int*)&buffer[i]);
			*(int*)&test[i] = pt;
		}
		unsigned char KC = Key & 0xFF;
		for (; i < size; i++) {
			test[i] = buffer[i] ^ KC;
		}
		memcpy(buffer, test, size);
		free(test);
	}
}

void ProtocolPacket::ChatEncode(int32_t EncodeKey) {
	int32_t size = this->size;

	//We don't want to include our crc
	if (HasCRC) {
		size -= 2;
	}

	if (buffer[1] != 0x01 && buffer[0] != 0x02 && buffer[0] != 0x1d) {
		int Key = EncodeKey;
		char *test = (char*)malloc(size);
		int i;
		buffer += 2;
		size -= 2;
		for (i = 0; i + 4 <= size; i += 4) {
			int pt = (*(int*)&buffer[i]) ^ (Key);
			Key = pt;
			*(int*)&test[i] = pt;
		}
		unsigned char KC = Key & 0xFF;
		for (; i < size; i++) {
			test[i] = buffer[i] ^ KC;
		}
		memcpy(buffer, test, size);
		free(test);
	}
}

/*bool ProtocolPacket::Combine(const ProtocolPacket *rhs) {
	bool result = false;
	//if(dont_combine)
	//	return false;
	//if (opcode==OP_Combined && size+rhs->size+5<256) {
	if (opcode == OP_Combined && (rhs->Size + 3) <= 255 && (rhs->Size + 3 + Size) < 512) {
		unsigned char *tmpbuffer = new unsigned char[Size + rhs->Size + 3];
		memcpy(tmpbuffer, buffer, Size);
		uint32_t offset = Size;
		tmpbuffer[offset++] = rhs->Size + 2;
		offset += rhs->serialize(tmpbuffer + offset);
		Size = offset;
		delete[] buffer;
		buffer = tmpbuffer;
		result = true;
	}
	else if (((Size + 3) < 255) && ((rhs->Size + 3) < 255)) {
		unsigned char *tmpbuffer = new unsigned char[Size + rhs->Size + 6];
		uint32_t offset = 0;
		tmpbuffer[offset++] = Size + 2;
		offset += serialize(tmpbuffer + offset);
		tmpbuffer[offset++] = rhs->Size + 2;
		offset += rhs->serialize(tmpbuffer + offset);
		Size = offset;
		delete[] buffer;
		buffer = tmpbuffer;
		opcode = OP_Combined;
		result = true;
	}
	return result;
}*/

EQ2Packet* ProtocolPacket::MakeApplicationPacket(uint8_t opcode_size) const {
	EQ2Packet* ret = nullptr;

	/*res->app_opcode_size = (opcode_size == 0) ? EQApplicationPacket::default_opcode_size : opcode_size;
	if (res->app_opcode_size == 1) {
		res->pBuffer = new unsigned char[size + 1];
		memcpy(res->pBuffer + 1, pBuffer, size);
		*(res->pBuffer) = htons(opcode) & 0xff;
		res->opcode = opcode & 0xff;
		res->size = size + 1;
	}
	else {
		res->pBuffer = new unsigned char[size];
		memcpy(res->pBuffer, pBuffer, size);
		res->opcode = opcode;
		res->size = size;
	}

	res->copyInfo(this);*/
	return ret;
}

void ProtocolPacket::WriteCRC(uint32_t Key) {
	if (!HasCRC) {
		return;
	}

	uint32_t dataLength = size - 2;

	uint16_t crc = static_cast<uint16_t>(CRC16(buffer, dataLength, Key));
	crc = htons(crc);
	memcpy(buffer + dataLength, &crc, 2);
}