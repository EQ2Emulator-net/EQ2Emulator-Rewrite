#include "EQ2Packet.h"
#include "../util.h"
#include "../Packets/EQ2Packets/OpcodeManager.h"
#include <zlib.h>
#include "../util.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Packet::EQ2Packet(uint16_t version) {
	app_opcode_size = 2;
	Version = version;

#ifdef DEBUG
	packet_buf = nullptr;
	packet_size = 0;
#endif
}

EQ2Packet::~EQ2Packet() {
#ifdef DEBUG
	if (packet_buf) {
		delete[] packet_buf;
	}
#endif
}

bool EQ2Packet::FindOpcode() {
	return OpcodeManager::GetGlobal()->SetOpcodeForPacket(this);
}

void EQ2Packet::HandlePacket(Client* client) {

}

uint8_t EQ2Packet::CompressPacket(z_stream& stream) {
	unsigned char* pDataPtr = buffer + 3;
	Bytef deflate_buff[4096];
	stream.next_in = pDataPtr;
	stream.avail_in = size - 3;

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

	//We don't necessarily need to trash the old buffer just truncate the size
	//remember to add the first 3 bytes we skipped over!
	size = total_bytes_written + 3;
	EQ2Compressed = true;

	//start_offset - 1, if this needs to change make sure you know what all is dependent on the return value
	return 2;
}

void EQ2Packet::EncryptPacket(Crypto& crypto) {
	if (size > 2 && crypto.isEncrypted()) {
		PacketEncrypted = true;
		crypto.RC4Encrypt(buffer + 2, size - 2);

		if (buffer[2] == 0) {
			unsigned char* new_buffer = new unsigned char[size + 1];
			//The first 2 bytes are the sequence which we don't need to set here
			//memcpy(new_buffer, buffer, 2);
			new_buffer[2] = 0;
			memcpy(new_buffer + 3, buffer + 2, size - 2);
			delete[] buffer;
			buffer = new_buffer;
			size += 1;
		}
	}
}

EQ2Packet* EQ2Packet::Create(const unsigned char* buf, uint32_t length, Crypto& crypto, uint16_t version) {
	uint32_t offset = 1;
	if (buf[0] == 0 && buf[1] == 0) {
		//Zero opcode packet
		length -= 1;
		offset += 1;
	}

	if (crypto.isEncrypted()) {
		crypto.RSADecrypt(buf, length);
	}

	uint16_t dataOp = buf[0];
	if (dataOp == 255) {
		//oversized
		memcpy(&dataOp, buf + 1, 2);
		offset += 2;
	}

	EQ2Packet* p = OpcodeManager::GetGlobal()->GetPacketForVersion(version, dataOp);
	if (p) {
		p->Read(buf + offset, 0, length);

#ifdef DEBUG
		unsigned char* cpy = new unsigned char[length];
		memcpy(cpy, buf + offset, length);
		p->packet_buf = cpy;
		p->packet_size = length;
#endif
	}

	return p;
}

void EQ2Packet::DumpBytes() {
	//add 4 bytes for protocol op and sequence just to keep it looking consistent
	uint32_t outsize = CalculateSize() + 4;

	unsigned char* tmp = new unsigned char[outsize];
	memset(tmp, 0, 4);
	tmp[1] = 0x09;
	Write(tmp + 4);
	::DumpBytes(tmp, outsize);
	delete[] tmp;
}

#ifdef DEBUG
void EQ2Packet::DumpPacket() {
	if (packet_buf) {
		::DumpBytes(packet_buf, packet_size);
	}
}
#endif