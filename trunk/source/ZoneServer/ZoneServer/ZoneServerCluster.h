#pragma once

#include "../../common/UDPServer.h"

/// <summary>This will handle all the incoming protocol packets from all the clients</summary>
class ZoneServerCluster : public UDPServer {
public:
	ZoneServerCluster();
	~ZoneServerCluster();

	bool Process() override;
	bool ProcessClientWrite();
	bool ProcessClients();

private:

};