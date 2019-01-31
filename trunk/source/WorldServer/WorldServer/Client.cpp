#include "Client.h"

#include "../../common/log.h"

Client::Client(unsigned int ip, unsigned short port) : EQ2Stream(ip, port) {

}

void Client::Process() {
	while (EQ2Packet* p = PopPacket()) {
		p->HandlePacket(this);
		delete p;
	}
}