#pragma once

#include "../../common/Server.h"

class WorldServer : public Server {
public:

	bool ProcessClientWrite();

protected:
	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

private:

};