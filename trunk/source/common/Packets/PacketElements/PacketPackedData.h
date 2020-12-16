#pragma once

#include "PacketElements.h"
#include "../../log.h"
#include <type_traits>

//You may choose to either inherit from this class or simply use PacketPackedData::LinkSubstruct in the desired order of your packed data
class PacketPackedData : public PacketSubstruct {
	friend class XmlStructDumper;
public:
	PacketPackedData(bool bClassicClient, bool _bIncludeSize = true) : PacketSubstruct(0, true),
		lastPackedSize(0), bIncludeSize(_bIncludeSize), bBufInitialized(false), bClassic(bClassicClient) {}

	~PacketPackedData() = default;

	uint32_t lastPackedSize;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		CheckElementsInitialized();

		lastPackedSize = 0;

		if (offset + (bIncludeSize ? 4 : 0) > bufsize) {
			return false;
		}
		else if (bIncludeSize) {
			memcpy(&lastPackedSize, srcbuf + offset, 4);
			offset += 4;
		}

		if (lastPackedSize == 0) {
			return true;
		}

		if (offset + lastPackedSize > bufsize) {
			return false;
		}

		std::vector<unsigned char> tmpSrc(lastPackedSize);

		memcpy(tmpSrc.data(), srcbuf + offset, lastPackedSize);
		offset += lastPackedSize;

		std::vector<unsigned char> tmpDst(32768);

		int32_t outsize = 0;

		if (!DoUnpack(lastPackedSize, tmpSrc.data(), outsize, tmpDst.data(), static_cast<uint32_t>(tmpDst.size()), !bClassic)) {
			LogDebug(LOG_PACKET, 0, "PacketPackedData::ReadElement unable to unpack data!");
			return false;
		}

		tmpSrc.clear();

		uint32_t readOffset = 0;

		for (auto& itr : elements) {
			if (!itr->MeetsCriteria()) {
				continue;
			}

			if (!itr->ReadElement(tmpDst.data(), readOffset, outsize)) {
				return false;
			}
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		assert(bBufInitialized);
		if (buf.empty()) {
			return;
		}
		memcpy(outbuf + offset, buf.data(), buf.size());
		offset += static_cast<uint32_t>(buf.size());
		bBufInitialized = false;
	}

	uint32_t GetSize() override {
		CheckElementsInitialized();

		if (bBufInitialized) {
			return static_cast<uint32_t>(buf.size());
		}

		uint32_t unpackedSize = 0;
		for (auto& e : elements) {
			if (!e->MeetsCriteria()) {
				continue;
			}

			unpackedSize += e->GetSize();
		}

		uint32_t nSizeBytes = (bIncludeSize ? 4 : 0);

		std::vector<unsigned char> tmp;
		tmp.resize(unpackedSize + nSizeBytes);
		buf.resize(unpackedSize + nSizeBytes);

		uint32_t offset = 0;
		for (auto& e : elements) {
			if (!e->MeetsCriteria()) {
				continue;
			}

			e->WriteElement(tmp.data(), offset);
		}

		uint32_t packedSize;
		if (unpackedSize == 0) {
			packedSize = 0;
		}
		else {
			packedSize = DoPack(buf.data(), tmp.data(), unpackedSize, static_cast<uint32_t>(buf.size()), !bClassic) - 4;
		}

		uint32_t effectiveSize = nSizeBytes;
		if (bIncludeSize) {
			memcpy(tmp.data(), &packedSize, nSizeBytes);
		}
		tmp.resize(packedSize + effectiveSize);
		if (packedSize > 0) {
			memcpy(tmp.data() + effectiveSize, buf.data() + 4, packedSize);
		}
		buf = std::move(tmp);
		bBufInitialized = true;
		return packedSize + effectiveSize;
	}

	static void DoPackReverse(uint8_t* input, uint32_t srcLen) {
		uint16_t real_pos = 0;
		uint16_t orig_pos = 0;
		uint8_t reverse_count = 0;
		while (srcLen > 0 && srcLen < 0xFFFFFFFF) { // XXX it was >=0 before. but i think it was a bug
			uint8_t code = input[real_pos++];
			srcLen--;
			if (code >= 128) {
				for (uint8_t index = 0; index < 7; index++) {
					if (code & 1) {
						if (srcLen >= 0 && !srcLen--)
							return;
						real_pos++;
						reverse_count++;
					}
					code >>= 1;
				}
			}
			if (reverse_count > 0) {
				uint8_t tmp_data[8] = { 0 };
				for (uint8_t i = 0; i < reverse_count; i++) {
					tmp_data[i] = input[orig_pos + reverse_count - i];
				}
				memcpy(input + orig_pos + 1, tmp_data, reverse_count);
				reverse_count = 0;
			}
			orig_pos = real_pos;
		}
	}

	static uint32_t DoPack(uint8_t* data, uint8_t* src, uint16_t srcLen, uint16_t dstLen, bool bReverse) {
		uint16_t	real_pos = 4;
		uint32_t	pos = 0;
		uint32_t	code = 0;
		int32_t		codePos = 0;
		int32_t		codeLen = 0;
		uint8_t	    zeroLen = 0;
		memset(data, 0, dstLen);

		while (pos < srcLen) {
			if (src[pos] || codeLen) {
				if (!codeLen) {
					if (zeroLen) {
						data[real_pos++] = zeroLen;
						zeroLen = 0;
					}
					codePos = real_pos;
					code = 0;
					data[real_pos++] = 0;
				}
				if (src[pos]) {
					data[real_pos++] = src[pos];
					code |= 0x80;
				}
				code >>= 1;
				codeLen++;

				if (codeLen == 7) {
					data[codePos] = uint8_t(0x80 | code);
					codeLen = 0;
				}
			}
			else {
				if (zeroLen == 0x7F) {
					data[real_pos++] = zeroLen;
					zeroLen = 0;
				}
				zeroLen++;
			}
			pos++;
		}
		if (codeLen) {
			code >>= (7 - codeLen);
			data[codePos] = uint8_t(0x80 | code);
		}
		else if (zeroLen) {
			data[real_pos++] = zeroLen;
		}
		if (bReverse) {
			DoPackReverse(data + 4, real_pos - 4);
		}
		uint32_t dataLen = real_pos - 4;
		memcpy(&data[0], &dataLen, sizeof(uint32_t));
		return dataLen + 4;
	}

	static bool DoUnpack(uint32_t srcLen, uint8_t* data, int32_t& outsize, uint8_t* dst, uint16_t dstLen, bool reverse) {
		if (reverse)
			DoPackReverse(data, srcLen);
		uint32_t dst_pos = 0;
		uint32_t src_pos = 0;
		while (src_pos < srcLen) {
			uint8_t code = data[src_pos++];

			if (code >= 128) {
				for (uint8_t index = 0; index < 7; index++) {
					if (dst_pos >= dstLen) {
						return false;
					}

					if (code & 1) {
						if (src_pos >= srcLen) {
							return false;
						}

						dst[dst_pos++] = data[src_pos++];
					}
					else {
						dst[dst_pos++] = 0;
					}
					code >>= 1;
				}
			}
			else {
				if (dst_pos + code > dstLen)
					return false;
				memset(dst + dst_pos, 0, code);
				dst_pos += code;
			}
		}

		outsize = dst_pos;
		return src_pos == srcLen;
	}

	//This will add an external substruct to this packed data's elements list
	template <typename T>
	void LinkSubstruct(T& ps, const char* element_name) {
		static_assert(std::is_base_of<PacketSubstruct, T>::value, "PacketPacketData::LinkSubstruct must take a PacketSubstruct");

		auto e = new PacketSubstructParent<T>(ps);
		PushElementForRegistration(e, element_name, std::remove_pointer_t<decltype(e)>);
	}

private:
	std::vector<unsigned char> buf;
	bool bBufInitialized;
	bool bClassic;
	bool bIncludeSize;
};