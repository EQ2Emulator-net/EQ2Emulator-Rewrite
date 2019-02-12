#include "stdafx.h"

#include "Client.h"

#include "../../common/log.h"
#include "../Database/WorldDatabase.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/EQ2Packets/OP_LoginReplyMsg_Packet.h"
#include "WorldServer.h"
#include "../Packets/OP_LoginRequestMsg_Packet.h"

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

void Client::ReadVersionPacket(unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) {
	//Since this packet is what sets the version and that moves around, we need to try and determine the struct
	//Find the approximate size of the packet not including strings to take a guess
	string tmp;
	Packet16String e(tmp);

	uint32_t tmp_offset = offset;

	for (int i = 0; i < 4; i++) {
		e.ReadElement(data, tmp_offset, size);
	}

	uint32_t remaining_size = size - tmp_offset;

	//Factor out the STATION string16 that gets sent for most client versions except really early ones
	if (remaining_size >= 9) {
		//7 char bytes + the 2 byte size
		remaining_size -= 9;
	}

	//21 Bytes is the remaining size for the 1208 client, I'm assuming the largest struct before the change
	uint16_t struct_version = remaining_size > 21 ? 1212 : 1;

	//We want to handle this packet now because other packets rely on the version set from it
	OP_LoginRequestMsg_Packet p(struct_version);


	p.Read(data, offset, size);
	p.HandlePacket(this);
}