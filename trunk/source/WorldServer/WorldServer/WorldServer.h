#pragma once

#include "../../common/Server.h"

class WorldServer : public Server {
public:

protected:
	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

private:

};