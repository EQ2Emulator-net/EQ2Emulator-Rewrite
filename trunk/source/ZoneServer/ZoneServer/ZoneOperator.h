#pragma once

#include "../../common/UDPServer.h"

/// <summary>This will handle all the incoming protocol packets from all the clients</summary>
class ZoneOperator : public UDPServer {
public:
	ZoneOperator();
	~ZoneOperator();

	bool Process() override;
	bool ProcessClientWrite();
	bool ProcessClients();

	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

private:

};