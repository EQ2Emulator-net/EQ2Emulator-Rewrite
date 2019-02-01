#pragma once

#include "../../common/EQ2Stream.h"

class Client : public EQ2Stream {
public:
	Client(unsigned int ip, unsigned short port);

	void Process();

	void SetAccount(uint32_t id) { AccountID = id; }
	uint32_t GetAccountID() { return AccountID; }

	void LogIn(std::string user, std::string pass);

protected:

private:
	void SendLoginReply(uint8_t reply);

	uint32_t AccountID;

};