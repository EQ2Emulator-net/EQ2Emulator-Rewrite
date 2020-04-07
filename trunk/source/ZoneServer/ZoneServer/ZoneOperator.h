#pragma once

#include <set>
#include "../../common/UDPServer.h"

class Client;
class ZoneServer;
class OP_LoginByNumRequestMsg_Packet;
class OP_SetRemoteCmdsMsg_Packet;

struct PendingClient {
	uint32_t access_code;
	uint32_t character_id;
	uint32_t zone_id;
	uint32_t instance_id;
};

struct Command {
	std::string name;
	// uint32_t = id
	std::map<uint32_t, std::string> subcommands;
};

/// <summary>This will handle all the incoming protocol packets from all the clients</summary>
class ZoneOperator : public UDPServer {
public:
	ZoneOperator();
	~ZoneOperator();

	bool Process() override;
	void AddStream(std::shared_ptr<Stream> stream, std::string key) override;
	bool ProcessClientWrite();
	bool ProcessClients();

	std::shared_ptr<Stream> GetNewStream(unsigned int ip, unsigned short port) override;

	void ClientLogIn(std::shared_ptr<Client> client, OP_LoginByNumRequestMsg_Packet* packet);

	void AddPendingClient(uint32_t account_id, PendingClient pending_client);

	std::shared_ptr<ZoneServer> AddNewZone(uint32_t zone_id, uint32_t instance_id);

	std::shared_ptr<ZoneServer> GetZone(uint32_t zone_id, uint32_t instance_id);

	void AddCommand(uint32_t id, std::string name);
	void AddSubCommand(std::string parent_cmd, uint32_t id, std::string name);
	OP_SetRemoteCmdsMsg_Packet* GetCommandsPacket(uint32_t version);

private:
	//Just using the client pointer as a key
	std::set<std::weak_ptr<Client>, std::owner_less<std::weak_ptr<Client> > > clients;
	// key = account id
	std::map<uint32_t, PendingClient> pending_clients;
	// pair<zoneID, instanceID>
	std::map<std::pair<uint32_t, uint32_t>, std::shared_ptr<ZoneServer> > zones;

	std::map<uint32_t, Command*> commands;
	OP_SetRemoteCmdsMsg_Packet* commands_packet;
};
