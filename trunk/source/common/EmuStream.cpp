#include "stdafx.h"

#include "EmuStream.h"
#include "util.h"

//Temp for now, will add more shtuff
struct EmuPacket {
	uint32_t length;
	unsigned char* data;

	~EmuPacket() {
		delete[] data;
	}
};

EmuStream::~EmuStream() {
	while (!incoming.empty()) {
		EmuPacket* p = incoming.front();
		delete p;
		incoming.pop();
	}

	while (!outgoing.empty()) {
		EmuPacket* p = outgoing.front();
		delete p;
		outgoing.pop();
	}
}

void EmuStream::Process(const unsigned char* data, unsigned int length) {
	//Create an emu packet...

	//Temp for now
	auto p = new EmuPacket;
	p->data = new unsigned char[length];
	memcpy(p->data, data, length);
	p->length = length;
	incoming.push(p);
}

void EmuStream::QueuePacket(const unsigned char* data, uint32_t length) {
	auto p = new EmuPacket;
	p->data = new unsigned char[length];
	memcpy(p->data, data, length);
	p->length = length;
	outgoing.push(p);
}

void EmuStream::Process() {
	while (!outgoing.empty()) {
		EmuPacket* p = outgoing.front();

		WritePacket(Sock, p->data, p->length);

		delete p;
		outgoing.pop();
	}

	while (!incoming.empty()) {
		EmuPacket* p = incoming.front();

		DumpBytes(p->data, p->length);

		delete p;
		incoming.pop();
	}
}