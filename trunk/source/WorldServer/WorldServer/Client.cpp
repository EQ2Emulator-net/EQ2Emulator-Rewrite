#include "Client.h"

#include "../../common/log.h"

Client::Client(unsigned int ip, unsigned short port) : EQ2Stream(ip, port) {

}

bool Client::HandlePacket() {
	return true;
}