#include "stdafx.h"

#include "WorldTalk.h"
#include "WorldStream.h"

std::shared_ptr<Stream> WorldTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return std::make_shared<WorldStream>(ip, port);
}

bool WorldTalk::Process() {
	ReadLocker lock(streamLock);
	for (auto& itr : Streams) {
		itr.second->Process();
	}

	return true;
}