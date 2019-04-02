#pragma once

#include "../../common/TCPServer.h"
#include <map>
#include <string>

class ZoneStream;
class OP_PlayCharacterReplyMsg_Packet;

class ZoneTalk : public TCPServer {
public:
	ZoneTalk();
	~ZoneTalk() = default;

	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

	bool Process() override;

	void RegisterZoneServer(ZoneStream* stream, std::string ip, uint16_t port);

	void StreamDisconnected(Stream* stream) override;

	bool GetAvailableZone(OP_PlayCharacterReplyMsg_Packet* p);

private:
	//This is the UDP connection info for this server, not the connection for the TCP stream
	//Will probably replace this with a more detailed structure later
	struct ZoneListeningInfo {
		std::string ip;
		uint16_t port;
	};

	Mutex m_zones;
	std::map<SOCKET, ZoneListeningInfo> zones;
};