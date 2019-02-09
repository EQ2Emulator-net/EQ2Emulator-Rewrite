#include "stdafx.h"

#include "WorldServer.h"
#include "Client.h"

WorldServer::WorldServer() {
	Name = "Rewrite Test Server";
	AllowedRaces = 0x001FFFFF;
	AllowedClasses = 0x07FFFFFE;
	MaxCharactersPerAccount = 20;
	MaxLevel = 50;
}

Stream* WorldServer::GetNewStream(unsigned int ip, unsigned short port) {
	return new Client(ip, port);
}

bool WorldServer::Process() {
	return ProcessClientWrite() && ProcessClients();
}

bool WorldServer::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Write();
	}

	return true;
}

bool WorldServer::ProcessClients() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Process();
	}

	return true;
}