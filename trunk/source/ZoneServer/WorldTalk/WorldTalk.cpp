#include "stdafx.h"

#include "WorldTalk.h"
#include "WorldStream.h"

Stream* WorldTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return new WorldStream(ip, port);
}

bool WorldTalk::Process() {
	ReadLocker lock(streamLock);
	for (auto& itr : Streams) {
		itr.second->Process();
	}

	return true;
}