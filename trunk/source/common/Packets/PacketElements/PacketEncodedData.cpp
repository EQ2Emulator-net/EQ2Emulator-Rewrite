#include "stdafx.h"

#include "PacketEncodedData.h"
#include "../../../ZoneServer/Spawns/SpawnStructs.h"
#include "../../EmuAssert.h"

void PacketEncodedData::EncodeData() {
	if (b_write_buf_initialized) {
		return;
	}

	//If you fail this assert, make sure you are setting the buffer with SetEncodedBuffer()
	EmuAssert(encodingBuf);
	GetSize();
	buf.resize(element_size);
	uint32_t tmp = 0;
	PacketSubstruct::WriteElement(buf.data(), tmp);
	b_write_buf_initialized = true;
	encodingBuf->Encode(buf.data(), element_size);
}

bool PacketEncodedData::DecodedRead() {
	uint32_t tmp = 0;
	return PacketSubstruct::ReadElement(buf.data(), tmp, static_cast<uint32_t>(buf.size()));
}

bool PacketEncodedData::DecodeData(std::shared_ptr<EncodedBuffer>& xorBuf) {
	if (!b_ready_for_decode) {
		LogError(LOG_PACKET, 0, "Tried to decode a packet that was not read properly!");
		return false;
	}
	b_ready_for_decode = false;

	xorBuf->Decode(buf.data(), static_cast<uint32_t>(buf.size()));
	return DecodedRead();
}

bool PacketEncodedData::ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
	if (!b_size_initialized) {
		GetSize();
	}

	b_size_initialized = false;

	buf.resize(element_size);

	if (!BoundsCheckRead(offset, bufsize)) {
		return false;
	}

	memcpy(buf.data(), srcbuf + offset, element_size);
	offset += element_size;
	b_ready_for_decode = true;
	b_size_initialized = false;
	return true;
}

uint32_t PacketEncodedData::GetSize() {
	if (!b_size_initialized) {
		element_size = PacketSubstruct::GetSize();
		b_size_initialized = true;
	}
	return element_size;
}

void PacketEncodedData::WriteElement(unsigned char* outbuf, uint32_t& offset) {
	EncodeData();

	EmuAssert(b_write_buf_initialized);
	b_write_buf_initialized = false;

	memcpy(outbuf + offset, buf.data(), buf.size());

	offset += static_cast<uint32_t>(buf.size());
}