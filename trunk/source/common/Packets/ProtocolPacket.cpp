#include "ProtocolPacket.h"
#include "ProtocolPackets/ProtocolPackets.h"

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