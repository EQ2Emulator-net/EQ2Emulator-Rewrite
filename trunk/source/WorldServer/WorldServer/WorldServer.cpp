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

	for (auto& itr : Streams) {
		static_cast<Client*>(itr.second.get())->Write();
	}

	return true;
}

bool WorldServer::ProcessClients() {
	ReadLocker lock(streamLock);

	for (auto& itr : Streams) {
		static_cast<Client*>(itr.second.get())->Process();
	}

	return true;
}