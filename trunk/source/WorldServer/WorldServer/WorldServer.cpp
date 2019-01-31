#include "WorldServer.h"
#include "Client.h"

Stream* WorldServer::GetNewStream(unsigned int ip, unsigned short port) {
	return new Client(ip, port);
}

bool WorldServer::ProcessClientWrite() {
	bool ret = true;

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Write();
	}

	return ret;
}

bool WorldServer::ProcessClients() {
	bool ret = true;

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Process();
	}

	return ret;
}