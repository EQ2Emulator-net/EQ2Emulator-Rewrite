#include "EQ2Packet.h"
#include "../util.h"
#include "../Packets/EQ2Packets/OpcodeManager.h"
#include <zlib.h>

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Packet::EQ2Packet(uint16_t version) {
	app_opcode_size = 2;
	Version = version;
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