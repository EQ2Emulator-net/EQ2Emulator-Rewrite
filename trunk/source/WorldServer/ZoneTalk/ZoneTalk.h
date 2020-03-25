#pragma once

#include "../../common/TCPServer.h"
#include <map>
#include <string>

class ZoneStream;
class OP_PlayCharacterReplyMsg_Packet;
class Client;

class ZoneTalk : public TCPServer {
public:
	ZoneTalk();
	~ZoneTalk() = default;

	std::shared_ptr<Stream> GetNewStream(unsigned int ip, unsigned short port) override;

	bool Process() override;

	void StreamDisconnected(std::shared_ptr<Stream> stream) override;

	bool GetAvailableZone(std::shared_ptr<Client> client, uint32_t char_id);

	void AddZone(std::shared_ptr<ZoneStream> zs, uint32_t zone_id, uint32_t instance_id);

	void TransferClientToZone(std::shared_ptr<ZoneStream> zs, std::shared_ptr<Client> client, uint32_t zone_id, uint32_t instance_id);

private:
	std::map<uint32_t, std::vector<std::weak_ptr<Client> > > pendingClientZones;
};