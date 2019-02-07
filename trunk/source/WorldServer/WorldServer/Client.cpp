#include "stdafx.h"

#include "Client.h"

#include "../../common/log.h"
#include "../Database/WorldDatabase.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"
#include "WorldServer.h"

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
		SendLoginReply(0);
	else
		SendLoginReply(1);
}

void Client::SendLoginReply(uint8_t reply) {
	// Login error, 0 = accepted, 1 = invalid password, 2 = currently playing, 6 = bad version, every thing else = unknown reason
	LogDebug(LOG_CLIENT, 0, "SendLoginReply called with %u", reply);

	OP_LoginReplyMsg_Packet* r = new OP_LoginReplyMsg_Packet(GetVersion());
	r->Response = reply;
	if (reply == 0) {
		r->AccountID = AccountID;
		r->ResetAppearance = 0;
		r->DoNotForceSoga = 1;
		r->RaceUnknown = 63;
		r->Unknown11 = 7;
		r->SubscriptionLevel = 2;
		r->RaceFlag = GetServer()->GetAllowedRaces();	//0x001FFFFF;
		r->ClassFlag = GetServer()->GetAllowedClasses();	//0x07FFFFFE;

		// Unknown5 and Unknown7 set to DoV values
		r->Unknown5 = 1148;
		r->Unknown7 = 2145009599;

		r->Unknown10 = 1;
		r->NumClassItems = 0;
		r->UnknownArraySize = 0;
	}
	else {		
		r->AccountID = 0xFFFFFFFF;
		r->ParentalControlTimer = 0xFFFFFFFF;
	}
	QueuePacket(r);
}

WorldServer* Client::GetServer() {
	return (WorldServer*)server;
}

void Client::SaveErrorsToDB(std::string log, std::string type) {
	uint32_t size = (uint32_t)log.size();
	char* message = new char[size];
	memset(message, 0, size);
	
	z_stream zstream;
	int zerror = 0;
	zstream.next_in = (BYTE*)log.c_str();
	zstream.avail_in = size;
	zstream.next_out = (BYTE*)message;
	zstream.avail_out = size;
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.opaque = Z_NULL;

	zerror = inflateInit(&zstream);
	if (zerror != Z_OK) {
		if (message)
			delete[] message;
		return;
	}
	zerror = inflate(&zstream, 0);
	if (message && strlen(message) > 0)
		database.SaveClientLog(type, message, GetVersion());

	if (message)
		delete[] message;
}