#pragma once

#include "../../common/TCPServer.h"
#include <map>
#include <string>
#include <set>

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

	std::shared_ptr<ZoneStream> GetAvailableZone(uint32_t zone_id);

	void AddZone(std::shared_ptr<ZoneStream> zs, uint32_t zone_id, uint32_t instance_id);

	uint32_t TransferClientToZone(std::shared_ptr<ZoneStream> zs, uint32_t character_id, uint32_t zone_id, uint32_t account_id,
		uint32_t instance_id, bool bFromZone);

	bool HasPendingZone(uint32_t zone_id);

	void AddPendingSessionTransfer(uint32_t access_code, uint32_t sessionID, const std::shared_ptr<ZoneStream>& fromZs);

	struct PendingSessionTransfer {
		uint32_t sessionID;
		std::weak_ptr<ZoneStream> zs;
	};

	PendingSessionTransfer PopPendingSessionTransfer(uint32_t access_code);

	//Add a zone client to transfer to another zone
	void AddPendingZoneTransfer(uint32_t characterID, uint32_t zoneID, uint32_t sessionID, uint32_t accountID);
	//Add a login client to transfer to a zone server
	void AddPendingZoneClient(uint32_t zoneID, const std::shared_ptr<Client>& client);

private:
	std::map<uint32_t, std::weak_ptr<ZoneStream> > pendingZones;
	std::map<uint32_t, std::vector<std::weak_ptr<Client> > > pendingZoneClients;

	struct PendingZoneTransfer {
		uint32_t characterID;
		uint32_t accountID;
	};
	//map<zone_id, map<session_id, PendingZoneTransfer>>
	std::map<uint32_t, std::map<uint32_t, PendingZoneTransfer> > pendingZoneTransfers;

	//map<access_code, PendingZoneTransfer>
	std::map<uint32_t, PendingSessionTransfer> pendingSessionTransfers;
};