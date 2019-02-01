#pragma once

#include "PacketElement.h"
#include <vector>
#include "../log.h"

class Packet {
public:
	virtual ~Packet() {
		if (buffer)
			delete[] buffer;

		for (size_t i = 0; i < elements.size(); i++)
			delete elements[i];
	}

	void Read(const unsigned char* in_buf, uint32_t offset, uint32_t bufsize) {
		for (size_t i = 0; i < elements.size(); i++) {
			if (!elements[i]->ReadElement(in_buf, offset, bufsize)) {
				LogError(LOG_PACKET, 0, "Reading an element went out of bounds");
				break;
			}
		}
	}

	uint32_t Write() {
		uint32_t size = CalculateSize();

		if (buffer && this->size != size) {
			delete[] buffer;
			this->size = size;
		}

		buffer = new unsigned char[size];

		return Write(buffer);
	}

	virtual uint32_t Write(unsigned char* writeBuffer) {
		//Should probably do a size check here but we won't ever see this code so do it yourself, YOLO!
		uint32_t offset = 0;
		for (auto& e : elements) {
			e->WriteElement(buffer, offset);
		}

		//offset is effectively the number of bytes written
		return offset;
	}

	const unsigned char* GetBuffer() {
		return buffer;
	}

	uint32_t GetBufferSize() {
		return size;
	}

	virtual uint32_t CalculateSize() {
		uint32_t ret = 0;
		for (auto& e : elements) {
			ret += e->GetSize();
		}
		return ret;
	}

protected:
	Packet() : buffer(nullptr), size(0) {
	}
	std::vector<PacketElement*> elements;

	unsigned char* buffer;
	uint32_t size;
};