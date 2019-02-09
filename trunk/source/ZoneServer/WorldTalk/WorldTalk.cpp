#include "stdafx.h"

#include "WorldTalk.h"
#include "WorldStream.h"

Stream* WorldTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return new WorldStream(ip, port);
}