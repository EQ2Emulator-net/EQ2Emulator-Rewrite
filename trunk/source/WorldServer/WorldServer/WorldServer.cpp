#include "WorldServer.h"
#include "Client.h"

Stream* WorldServer::GetNewStream(unsigned int ip, unsigned short port) {
	return new Client(ip, port);
}