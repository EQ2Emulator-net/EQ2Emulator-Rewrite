#include "stdafx.h"

#include "ZoneTalk.h"
#include "ZoneStream.h"

ZoneTalk::ZoneTalk() : TCPServer(true) {

}

Stream* ZoneTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return new ZoneStream(ip, port);
}