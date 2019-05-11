#include "stdafx.h"

#include "ZoneOperator.h"
#include "Client.h"

ZoneOperator::ZoneOperator() {

}

ZoneOperator::~ZoneOperator() {

}

bool ZoneOperator::Process() {
	return ProcessClientWrite() && ProcessClients(); // ProcessClients should be moved to ZoneServer in the future
}

bool ZoneOperator::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Write();
	}

	return true;
}

bool ZoneOperator::ProcessClients() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Process();
	}

	return true;
}

Stream* ZoneOperator::GetNewStream(unsigned int ip, unsigned short port) {
	return new Client(ip, port);
}