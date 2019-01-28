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

	void Read(unsigned char* in_buf, uint32_t off, uint32_t bufsize) {
		buffer = in_buf;
		offset = off;
		for (size_t i = 0; i < elements.size(); i++) {
			if (!elements[i]->ReadElement(buffer, offset, bufsize)) {
				LogError(LOG_PACKET, 0, "Reading an element went out of bounds");
				break;
			}
		}
	}

	virtual uint32_t Write(unsigned char*& writeBuffer) {
		uint32_t size = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			size += elements[i]->GetSize();
		}

		if (buffer)
			delete[] buffer;

		buffer = new unsigned char[size];
		offset = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			elements[i]->WriteElement(buffer, offset);
		}

		writeBuffer = buffer;

		return size;
	}

protected:
	Packet() : buffer(nullptr) {
		offset = 0;
	}

	unsigned char* buffer;
	uint32_t offset;
	std::vector<PacketElement*> elements;
};