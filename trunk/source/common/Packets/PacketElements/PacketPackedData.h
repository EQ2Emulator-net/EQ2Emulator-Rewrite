#pragma once

#include "PacketElements.h"
#include "../../log.h"
#include <type_traits>

void DoPackReverse(uint8_t* input, uint32_t srcLen) {
	uint16_t real_pos = 0;
	uint16_t orig_pos = 0;
	uint8_t reverse_count = 0;
	while (srcLen > 0 && srcLen < 0xFFFFFFFF) { // XXX it was >=0 before. but i think it was a bug
		uint8_t code = input[real_pos++];
		srcLen--;
		if (code >= 128) {
			for (uint8_t index = 0; index<7; index++) {
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
			for (uint8_t i = 0; i<reverse_count; i++) {
				tmp_data[i] = input[orig_pos + reverse_count - i];
			}
			memcpy(input + orig_pos + 1, tmp_data, reverse_count);
			reverse_count = 0;
		}
		orig_pos = real_pos;
	}
}

uint32_t DoPack(uint8_t* data, uint8_t* src, uint16_t srcLen, uint16_t dstLen) {
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
				if (zeroLen > 5) {
					data[real_pos++] = zeroLen;
					zeroLen = 0;
				}
				else if (zeroLen >= 1 && zeroLen <= 5) {
					for (; zeroLen>0; zeroLen--)
						codeLen++;
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
	DoPackReverse(data + 4, real_pos - 4);
	uint32_t dataLen = real_pos - 4;
	memcpy(&data[0], &dataLen, sizeof(uint32_t));
	return dataLen + 4;
}

uint32_t DoPackClassic(uint8_t* input, int32_t inputSize, uint8_t* outBuf, int32_t outBufSize) {
	assert(outBufSize > 4);
	int32_t inputCount = inputSize;
	uint8_t * outBufStart = outBuf;
	outBufSize -= 4;
	outBuf += 4;
	memset(outBuf, 0, outBufSize);

	while (inputCount) {
		int32_t code = min<>(7, inputCount);
		int32_t zeroLen = 0;
		zeroLen = 0;
		uint8_t bVar5 = 0;

		auto ZeroFillBlock = [&code, &zeroLen, &input, &inputCount, &outBuf] {
			inputCount -= code;
			while ((input[zeroLen] == '\0' && zeroLen < 0x7f) && inputCount) {
				++zeroLen;
				--inputCount;
			}
			*outBuf = zeroLen;
			++outBuf;
		};

		if (code == 0) {
			ZeroFillBlock();
		}
		else {
			do {
				if (input[zeroLen] != '\0') {
					bVar5 = bVar5 | 1 << (zeroLen & 0x1f);
				}
				++zeroLen;
			} while (zeroLen < code);

			if (bVar5 == 0) {
				ZeroFillBlock();
			}
			else {
				*outBuf = bVar5 | 0x80;
				++outBuf;

				zeroLen = 0;
				do {
					if ((static_cast<uint32_t>(bVar5 | 0x80) & 1 << (zeroLen & 0x1f)) != 0) {
						*outBuf = input[zeroLen];
						++outBuf;
					}
					++zeroLen;
				} while (zeroLen < code);
				inputCount -= code;
				zeroLen = code;
			}
		}

		input += zeroLen;
	}

	uint32_t packSize = outBuf - outBufStart;
	memcpy(&outBufStart, &packSize, sizeof(uint32_t));

	return packSize + 4;
}

bool DoUnpack(uint32_t srcLen, uint8_t* data, uint32_t& outsize, uint8_t* dst, uint16_t dstLen, bool reverse) {
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

int32_t DoUnpackClassic(const uint8_t* input, int32_t inputSize, uint8_t* outBuf, int32_t outBufSize) {
	assert(outBufSize > 4);

	if (inputSize < 4) {
		return -1;
	}

	int32_t packedSize = *reinterpret_cast<const int32_t*>(input);

	if (packedSize == 0) {
		return 0;
	}

	if (4 + packedSize > inputSize) {
		return 0;
	}

	input += 4;
	inputSize -= 4;

	uint8_t* outStart = outBuf;
	outBuf += 4;
	outBufSize -= 4;

	uint32_t nOutRemaining = outBufSize;

	while (outBufSize != 0) {
		if (inputSize-- == 0) {
			break;
		}

		uint8_t code = *input;
		++input;
		if (code & 0x80) {
			uint8_t zeroLen = 0;
			bool bBreak = false;
			do {
				if ((static_cast<uint32_t>(zeroLen) & 1 << (zeroLen & 0x1f)) == 0) {
					*outBuf = 0;
				}
				else {
					*outBuf = *input;
					++input;
					--inputSize;
				}
				++outBuf;
				if (--nOutRemaining == 0 || --inputSize == 0) {
					bBreak = true;
					break;
				}
				++zeroLen;
			} while (zeroLen < 7);

			if (bBreak) {
				break;
			}
		}
		else {
			if (nOutRemaining < code) {
				return -1;
			}

			uint8_t zeroLen = code / 4;
			uint8_t* ptr = outBuf;
			memset(ptr, 0, zeroLen);
			ptr += zeroLen;
			zeroLen = code % 4;
			memset(ptr, 0, zeroLen);
			outBuf += code;
			nOutRemaining -= code;
		}
		outBufSize = nOutRemaining;
	}

	uint32_t unpackedSize = outBuf - outStart - 4;
	memcpy(outStart, &unpackedSize, sizeof(unpackedSize));

	return unpackedSize + 4;
}

//You may choose to either inherit from this class or simply use PacketPackedData::LinkSubstruct in the desired order of your packed data
class PacketPackedData : public PacketSubstruct {
public:
	PacketPackedData(bool bClassicClient, uint32_t p_nSizeBytes = 4) : PacketSubstruct(0), lastPackedSize(0), nSizeBytes(p_nSizeBytes), bBufInitialized(false), bClassic(bClassicClient) {}

	~PacketPackedData() = default;

	uint32_t lastPackedSize;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		CheckElementsInitialized();

		lastPackedSize = 0;

		if (offset + nSizeBytes > bufsize) {
			return false;
		}

		memcpy(&lastPackedSize, srcbuf + offset, nSizeBytes);

		offset += nSizeBytes;

		if (lastPackedSize == 0) {
			return true;
		}

		if (offset + lastPackedSize > bufsize) {
			return false;
		}

		unsigned char tmpSrc[32768];

		if (lastPackedSize > sizeof(tmpSrc)) {
			LogDebug(LOG_PACKET, 0, "PacketPackedData::ReadElement tried to read a packet sized larger than its buffer!");
			return false;
		}

		memcpy(tmpSrc, srcbuf + offset, lastPackedSize);
		offset += lastPackedSize;

		unsigned char tmpDst[32768];

		uint32_t outsize;

		if (bClassic) {
			if (DoUnpackClassic(tmpSrc, lastPackedSize, tmpDst, sizeof(tmpDst)) < 0) {
				LogDebug(LOG_PACKET, 0, "PacketPackedData::ReadElement unable to unpack data!");
				return false;
			}
		}
		else {
			if (!DoUnpack(lastPackedSize, tmpSrc, outsize, tmpDst, sizeof(tmpDst), true)) {
				LogDebug(LOG_PACKET, 0, "PacketPackedData::ReadElement unable to unpack data!");
				return false;
			}
		}

		uint32_t readOffset = 0;

		for (auto& itr : elements) {
			if (!itr->MeetsCriteria()) {
				continue;
			}

			if (!itr->ReadElement(tmpDst, readOffset, outsize)) {
				return false;
			}
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		assert(bBufInitialized);
		memcpy(outbuf + offset, buf.data(), buf.size());
		offset += static_cast<uint32_t>(buf.size());
	}

	uint32_t GetSize() override {
		CheckElementsInitialized();

		uint32_t unpackedSize = 0;
		for (auto& e : elements) {
			if (!e->MeetsCriteria()) {
				continue;
			}

			unpackedSize += e->GetSize();
		}

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
		if (bClassic) {
			packedSize = DoPackClassic(buf.data(), buf.size(), tmp.data(), unpackedSize) - 4;
			
		}
		else {
			packedSize = DoPack(buf.data(), tmp.data(), unpackedSize, buf.size()) - 4;
		}
		memcpy(tmp.data(), &packedSize, nSizeBytes);
		memcpy(tmp.data() + nSizeBytes, buf.data() + 4, packedSize);
		tmp.resize(packedSize + nSizeBytes);
		buf.swap(tmp);
		bBufInitialized = true;
		return packedSize + nSizeBytes;
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
	uint32_t nSizeBytes;
};