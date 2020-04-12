#include "stdafx.h"

#include "EmuPacket.h"
#include "Packets/PacketElements/PacketElements.h"
#include <zlib.h>

EmuPacket::EmuPacket(EmuOpcode_t op)
	:opcode(op), bCompressed(false) {
}

uint32_t EmuPacket::Write(unsigned char*& retBuffer) {
	uint32_t size = 0;
	for (size_t i = 0; i < elements.size(); i++) {
		if (!elements[i]->MeetsCriteria()) {
			continue;
		}

		size += elements[i]->GetSize();
	}

	if (size > 512 - 10) {
		//Just go ahead and compress this packet
		bCompressed = true;
	}

	if (bCompressed) {
		//Opcode and bCompressed + Size of the packet data before compressionn
		offset = 6;
	}
	else {
		//Opcode and bCompressed
		offset = 2;
	}

	size += offset;

	if (buffer)
		delete[] buffer;

	buffer = new unsigned char[size];
	for (auto& itr : elements) {
		if (!itr->MeetsCriteria()) {
			continue;
		}

		itr->WriteElement(buffer, offset);
	}

	//Do compression if we need to
	if (bCompressed) {
		Bytef tmp[4096];
		uLongf destLen = sizeof(tmp);
		//Data length is the total packet size - opcode - bCompressed - uncompressed_size
		uLong dataLen = size - 6;
		
		assert(compress(tmp, &destLen, buffer + 6, dataLen) == Z_OK);

		memcpy(buffer + 6, tmp, destLen);

		size = destLen + 6;

		//Add the size to our packet
		memcpy(buffer + 2, &dataLen, 4);
	}

	//Set our own protocol data
	buffer[0] = static_cast<uint8_t>(opcode);
	buffer[1] = (bCompressed ? 0x01 : 0x00);

	retBuffer = buffer;

	Size = size;
	return size;
}

bool EmuPacket::Read(const unsigned char* in_buf, uint32_t bufsize) {
	Bytef decompressBuf[4096];
	std::unique_ptr<unsigned char[]> bufDeletor;

	const unsigned char* ptr = in_buf;

	bCompressed = in_buf[1] != 0;

	if (bCompressed) {
		offset = 0;
		uint32_t uncompressedSize = *reinterpret_cast<const uint32_t*>(in_buf + 2);

		Bytef* tmpBuf;
		uLongf destLen;

		if (uncompressedSize > sizeof(decompressBuf)) {
			tmpBuf = new unsigned char[uncompressedSize];
			ptr = tmpBuf;
			destLen = uncompressedSize;
			bufDeletor.reset(tmpBuf);
		}
		else {
			tmpBuf = decompressBuf;
			ptr = decompressBuf;
			destLen = sizeof(decompressBuf);
		}

		assert(uncompress(tmpBuf, &destLen, in_buf + 6, bufsize - 6) == Z_OK);

		bufsize = uncompressedSize;
	}
	else {
		offset = 2;
	}

	for (auto& itr : elements) {
		if (!itr->MeetsCriteria()) {
			continue;
		}
		if (!itr->ReadElement(ptr, offset, bufsize)) {
			LogError(LOG_PACKET, 0, "Reading an element went out of bounds");
			return false;
		}
	}

	return true;
}