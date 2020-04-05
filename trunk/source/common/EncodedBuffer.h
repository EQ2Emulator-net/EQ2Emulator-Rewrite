#pragma once

#include <vector>
#include <cstdint>
#include <utility>
#include "Mutex.h"

class EncodedBuffer {
public:
	EncodedBuffer() {
		bufLock.SetName("EncodedBuffer::bufLock");
	}

	const uint8_t* Encode(const uint8_t* data, uint32_t n) {
		if (n == 0) {
			return data;
		}

		//Align the buf to 4 bytes for quicker XORing
		uint32_t notAligned = (n % 4);
		if (notAligned) {
			n += 4 - notAligned;
		}

		if (buffer.size() < n) {
			buffer.resize(n);
		}

		//Convert n from byte count to DWORD count (minus one incase of buffer overrun with the input data)
		int count = n / 4;

		if (notAligned) {
			--count;
		}

		uint32_t* encoded = reinterpret_cast<uint32_t*>(buffer.data());
		const uint32_t* input = reinterpret_cast<const uint32_t*>(data);
		for (int i = 0; i < count; i++) {
			encoded[i] ^= input[i];
		}

		//Last DWORD if input data is not aligned
		if (notAligned) {
			uint32_t mask;
			if (notAligned == 1) {
				mask = 0xFF;
			}
			else if (notAligned == 2) {
				mask = 0xFFFF;
			}
			else { //notAligned is 3
				mask = 0xFFFFFF;
			}
			
			//XOR with just the relevant values
			encoded[count] ^= (input[count] & mask);
		}

		return buffer.data();
	}

	WriteLocker GetBufLock() {
		return std::move(WriteLocker(bufLock));
	}

private:
	std::vector<uint8_t> buffer;
	Mutex bufLock;
};