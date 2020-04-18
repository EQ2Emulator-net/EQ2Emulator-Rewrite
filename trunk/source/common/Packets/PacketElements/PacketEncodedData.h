#pragma once

#include "PacketElements.h"
#include "../../log.h"
#include "../../EncodedBuffer.h"
#include <vector>
#include <memory>

class PacketEncodedData : public PacketSubstruct {
public:
	PacketEncodedData(uint32_t version) : PacketSubstruct(version), element_size(0), b_size_initialized(false), 
		b_write_buf_initialized(false), b_ready_for_decode(false) {
	
	}

	~PacketEncodedData() = default;

	uint32_t GetSize();

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize);

	void WriteElement(unsigned char* outbuf, uint32_t& offset);

	bool DecodeData(std::shared_ptr<EncodedBuffer>& xorBuf);

	void EncodeData();

	void SetEncodedBuffer(const std::shared_ptr<EncodedBuffer>& buf) {
		encodingBuf = buf;
	}

private:
	std::shared_ptr<EncodedBuffer> encodingBuf;
	std::vector<unsigned char> buf;
	uint32_t element_size;
	bool b_size_initialized;
	bool b_write_buf_initialized;
	bool b_ready_for_decode;
};