#pragma once

#include <vector>
#include <cstdint>
#include <utility>
#include "Mutex.h"
#include "util.h"
#include "log.h"

class EncodedBuffer {
public:
	EncodedBuffer() {
		bufLock.SetName("EncodedBuffer::bufLock");
	}

	EncodedBuffer(const EncodedBuffer& rhs) {
		inputBuf = rhs.inputBuf;
		buffer = rhs.buffer;
	}

	EncodedBuffer& operator=(const EncodedBuffer& rhs) {
		inputBuf = rhs.inputBuf;
		buffer = rhs.buffer;
		return *this;
	}

	void Encode(uint8_t* data, uint32_t n) {
		WriteLocker lock(bufLock);
		if (inputBuf.size() < n) {
			inputBuf.resize(n);
		}
		//Save the unmodified input for our next encode
		memcpy(inputBuf.data(), data, n);
		if (buffer.size() > n) {
			if (inputBuf.size() < buffer.size()) {
				inputBuf.resize(buffer.size());
			}
			//Copy over any "Extra" data in the buffer we aren't dealing with on this XOR (for packets that vary in size)
			memcpy(inputBuf.data() + n, buffer.data() + n, buffer.size() - n);
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

		uint32_t* encoded = reinterpret_cast<uint32_t*>(data);
		const uint32_t* cipher = reinterpret_cast<const uint32_t*>(buffer.data());
		for (int i = 0; i < count; i++) {
			encoded[i] ^= cipher[i];
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
			encoded[count] ^= (cipher[count] & mask);
		}

		//Ready our input for the next encode
		buffer.swap(inputBuf);
	}

	void Decode(uint8_t* data, uint32_t n) {
		WriteLocker lock(bufLock);
		if (n == 0) {
			return;
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

		uint32_t* encoded = reinterpret_cast<uint32_t*>(data);
		uint32_t* cipher = reinterpret_cast<uint32_t*>(buffer.data());
		for (int i = 0; i < count; i++) {
			encoded[i] ^= cipher[i];
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
			encoded[count] ^= (cipher[count] & mask);
		}

		//Save our decoded data for the next decode
		memcpy(cipher, encoded, n);
	}

private:
	std::vector<uint8_t> buffer;
	std::vector<uint8_t> inputBuf;
	Mutex bufLock;
};