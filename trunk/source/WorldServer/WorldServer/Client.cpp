#include "Client.h"

#include "../../common/log.h"
#include "../Database/WorldDatabase.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"

extern WorldDatabase database;

Client::Client(unsigned int ip, unsigned short port) : EQ2Stream(ip, port) {

}

void Client::Process() {
	while (EQ2Packet* p = PopPacket()) {
		p->HandlePacket(this);
		delete p;
	}
}

void Client::LogIn(std::string user, std::string pass) {
	if (!OpcodeManager::GetGlobal()->HasVersion(GetVersion()))
		SendLoginReply(6);
	else if (database.GetAccount(this, user, pass))
		SendLoginReply(6);
	else
		SendLoginReply(1);
}

void Client::SendLoginReply(uint8_t reply) {
	// Login error, 0 = accepted, 1 = invalid password, 2 = currently playing, 6 = bad version, every thing else = unknown reason
	LogDebug(LOG_CLIENT, 0, "SendLoginReply called with %u", reply);
	if (reply == 0) {

	}
	else {
		OP_LoginReplyMsg_Packet* r = new OP_LoginReplyMsg_Packet(GetVersion());
		r->Response = reply;
		r->AccountID = 0xFFFFFFFF;
		r->ParentalControlTimer = 0xFFFFFFFF;
		QueuePacket(r);
	}
}