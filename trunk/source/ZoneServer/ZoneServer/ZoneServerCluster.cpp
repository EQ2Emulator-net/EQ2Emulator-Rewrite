#include "stdafx.h"

#include "ZoneServerCluster.h"
#include "Client.h"

ZoneServerCluster::ZoneServerCluster() {

}

ZoneServerCluster::~ZoneServerCluster() {

}

bool ZoneServerCluster::Process() {
	return ProcessClientWrite() && ProcessClients(); // ProcessClients should be moved to ZoneServer in the future
}

bool ZoneServerCluster::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Write();
	}

	return true;
}

bool ZoneServerCluster::ProcessClients() {
	ReadLocker lock(streamLock);

	std::map<std::string, Stream*>::iterator stream_itr;
	for (stream_itr = Streams.begin(); stream_itr != Streams.end(); stream_itr++) {
		((Client*)stream_itr->second)->Process();
	}

	return true;
}