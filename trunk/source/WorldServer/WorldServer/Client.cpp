#include "Client.h"

#include "../../common/log.h"

Client::Client(unsigned int ip, unsigned short port) : EQ2Stream(ip, port) {

}

void Client::Process() {
	EQ2Packet* p = nullptr;
	while (p = PopPacket()) {
		p->HandlePacket(this);
		delete p;
		p = nullptr;
	}
}