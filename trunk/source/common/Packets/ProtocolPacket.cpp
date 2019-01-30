#include "ProtocolPacket.h"
#include "ProtocolPackets/ProtocolPackets.h"
#include "../util.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

ProtocolPacket::ProtocolPacket() {
	HasCRC = false;
}

uint32_t ProtocolPacket::Write(unsigned char*& writeBuffer) {
	uint32_t size = 0;
	for (size_t i = 0; i < elements.size(); i++) {
		size += elements[i]->GetSize();
	}

	size += 2; // opcode
	
	if (HasCRC)
		size +=2;

	if (buffer)
		delete[] buffer;

	buffer = new unsigned char[size];

	uint16_t op = htons(opcode);
	memcpy(buffer, &op, 2);
	offset = 2;
	for (size_t i = 0; i < elements.size(); i++) {
		elements[i]->WriteElement(buffer, offset);
	}

	writeBuffer = buffer;

	return size;
}

ProtocolPacket* ProtocolPacket::GetProtocolPacket(unsigned char* in_buff, uint32_t len) {
	ProtocolPacket* ret = nullptr;;
	uint16_t opcode = ntohs(*(uint16_t*)in_buff);
	uint32_t offset = 2;

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
	default: {
		LogError(LOG_PACKET, 0, "Unknown protocol packet upcode %u", opcode);
		if (in_buff)
			delete[] in_buff;

		break;
	}
	}

	if (ret)
		ret->Read(in_buff, offset, len);

	return ret;
}

// Copy and paste from old code
uint32_t ProtocolPacket::Compress(const unsigned char *buffer, const uint32_t length, unsigned char *newbuf, uint32_t newbufsize) {
	uint32_t flag_offset = 1, newlength;
	newbuf[0] = buffer[0];
	if (buffer[0] == 0) {
		flag_offset = 2;
		newbuf[1] = buffer[1];
	}
	if (length > 30) {
		newlength = Deflate(const_cast<unsigned char *>(buffer + flag_offset), length - flag_offset, newbuf + flag_offset + 1, newbufsize);
		*(newbuf + flag_offset) = 0x5a;
		newlength += flag_offset + 1;
	}
	else {
		memmove(newbuf + flag_offset + 1, buffer + flag_offset, length - flag_offset);
		*(newbuf + flag_offset) = 0xa5;
		newlength = length + 1;
	}

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

void ProtocolPacket::ChatEncode(unsigned char *buffer, int size, int EncodeKey) {
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