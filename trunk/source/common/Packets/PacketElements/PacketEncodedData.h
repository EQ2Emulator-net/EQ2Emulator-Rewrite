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

	uint32_t GetSize() override {
		if (!b_size_initialized) {
			element_size = PacketSubstruct::GetSize();
			b_size_initialized = true;
		}
		return element_size;
	}

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		if (!b_size_initialized) {
			GetSize();
		}

		b_size_initialized = false;

		buf.resize(element_size);

		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(buf.data(), srcbuf + offset, element_size);
		b_ready_for_decode = true;
		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		EncodeData();

		assert(b_write_buf_initialized);
		b_write_buf_initialized = false;

		memcpy(outbuf + offset, buf.data(), buf.size());

		offset += static_cast<uint32_t>(buf.size());
	}

	bool DecodeData(std::shared_ptr<EncodedBuffer>& xorBuf) {
		if (!b_ready_for_decode) {
			LogError(LOG_PACKET, 0, "Tried to decode a packet that was not read properly!");
			return false;
		}
		b_ready_for_decode = false;

		uint32_t tmp = 0;
		WriteLocker lock = xorBuf->GetBufLock();
		const uint8_t* p = xorBuf->Encode(buf.data(), static_cast<uint32_t>(buf.size()));
		return PacketSubstruct::ReadElement(p, tmp, static_cast<uint32_t>(buf.size()));
	}

	void EncodeData() {
		//If you fail this assert, make sure you are setting the buffer with SetEncodedBuffer()
		assert(encodingBuf);
		GetSize();
		buf.resize(element_size);
		uint32_t tmp = 0;
		PacketSubstruct::WriteElement(buf.data(), tmp);
		b_write_buf_initialized = true;
		WriteLocker lock = encodingBuf->GetBufLock();
		memcpy(buf.data(), encodingBuf->Encode(buf.data(), element_size), element_size);
	}

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