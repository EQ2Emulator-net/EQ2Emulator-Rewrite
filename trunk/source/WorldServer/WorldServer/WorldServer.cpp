#include "stdafx.h"

#include "WorldServer.h"
#include "Client.h"

WorldServer::WorldServer() {
	ID = 0;
	Name = "Rewrite Test Server";
	Locked = false;
	CharacterSlotsPerAccount = 10;
	MaxAdvLevel = 50;
	MaxTSLevel = 50;
	AllowedRaces = 0x001FFFFF;
	AllowedClasses = 0x07FFFFFE;
	AutoAccountCreation = true;
}

std::shared_ptr<Stream> WorldServer::GetNewStream(unsigned int ip, unsigned short port) {
	return std::make_shared<Client>(ip, port);
}

bool WorldServer::Process() {
	return ProcessClientWrite() && ProcessClients();
}

bool WorldServer::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	std::map<std::string, std::shared_ptr<Stream> >::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		//std::shared_ptr<Client> client = std::static_pointer_cast<Client>(stream_itr->second);
		Client* client = (Client*)stream_itr->second.get();
		if (client)
			client->Write();
	}

	return true;
}

bool WorldServer::ProcessClients() {
	ReadLocker lock(streamLock);

	std::map<std::string, std::shared_ptr<Stream> >::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		std::static_pointer_cast<Client>(stream_itr->second)->Process();
	}

	return true;
}