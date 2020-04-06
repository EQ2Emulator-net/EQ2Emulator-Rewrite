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

	virtual bool Read(const unsigned char* in_buf, uint32_t off, uint32_t bufsize) {
		offset = off;
		for (size_t i = 0; i < elements.size(); i++) {
			if (!elements[i]->MeetsCriteria()) {
				continue;
			}
			if (!elements[i]->ReadElement(in_buf, offset, bufsize)) {
				LogError(LOG_PACKET, 0, "Reading an element went out of bounds");
				return false;
			}
		}
		return true;
	}

	virtual uint32_t Write(unsigned char*& writeBuffer) {
		return Write(writeBuffer, GetElementSize());
	}

	unsigned char* buffer;
	uint32_t Size;

protected:
	Packet() : buffer(nullptr) {
		offset = 0;
		Size = 0;
	}

	uint32_t offset;
	std::vector<PacketElement*> elements;

	uint32_t Write(unsigned char*& writeBuffer, uint32_t size) {
		if (buffer)
			delete[] buffer;

		buffer = new unsigned char[size];
		offset = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			if (!elements[i]->MeetsCriteria()) {
				continue;
			}

			elements[i]->WriteElement(buffer, offset);
		}

		writeBuffer = buffer;

		Size = size;
		return size;
	}

	uint32_t GetElementSize() {
		uint32_t size = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			if (!elements[i]->MeetsCriteria()) {
				continue;
			}

			size += elements[i]->GetSize();
		}
		return size;
	}
};