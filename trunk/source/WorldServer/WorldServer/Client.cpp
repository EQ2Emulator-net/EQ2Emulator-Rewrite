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
	AccountID = 0;
	AllowedRaces = 0;
	AllowedClasses = 0;
	CharacterSlots = 0;
	pending_zone = 0;
	pending_instance = 0;
	pending_character = 0;
}

void Client::Process() {
	while (EQ2Packet* p = PopPacket()) {
		p->HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
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
		r->RaceFlag = GetAllowedRaces();
		r->ClassFlag = GetAllowedClasses();

		if (GetVersion() >= 60100) {
			r->Unknown5 = 1532;
			r->Unknown7 = 4219469759;
			r->Unknown7a = 8388607;
			r->RaceUnknown = 255;
		}
		else if (GetVersion() >= 1096) {
			r->Unknown5 = 1148;
			r->Unknown7 = 2145009599;
		}

		// Equipment shown during normal character creation
		r->Unknown10 = 1;
		WorldServer* s = GetServer();
		std::map<uint8_t, std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem> >::iterator itr;
		std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem>::iterator itr2;
		for (itr = s->NormalEquipment.begin(); itr != s->NormalEquipment.end(); itr++) {
			OP_LoginReplyMsg_Packet::ClassItem ci;
			ci.ClassID = itr->first;
			for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
				ci.StartingItems.push_back(*itr2);

			r->ClassItems.push_back(ci);
		}

		// Equipment shown during heroic (lvl 90) character creation
		r->Unknown12 = 1;
		for (itr = s->LVL90Equipment.begin(); itr != s->LVL90Equipment.end(); itr++) {
			OP_LoginReplyMsg_Packet::ClassItem ci;
			ci.ClassID = itr->first;
			for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
				ci.StartingItems.push_back(*itr2);

			r->LVL90ClassItems.push_back(ci);
		}

		// Equipment shown during time locked character creation
		r->Unknown13 = 1;
		for (itr = s->TLEquipment.begin(); itr != s->TLEquipment.end(); itr++) {
			OP_LoginReplyMsg_Packet::ClassItem ci;
			ci.ClassID = itr->first;
			for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
				ci.StartingItems.push_back(*itr2);

			r->TimeLockedClassItems.push_back(ci);
		}
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
	zstream.next_in = (Bytef*)log.c_str();
	zstream.avail_in = size;
	zstream.next_out = (Bytef*)message;
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

	inflateEnd(&zstream);
}

uint32_t Client::GetAllowedRaces() {
	return (AllowedRaces == 0) ? GetServer()->GetAllowedRaces() : AllowedRaces;
}

uint32_t Client::GetAllowedClasses() {
	return (AllowedClasses == 0) ? GetServer()->GetAllowedClasses() : AllowedClasses;
}

uint8_t Client::GetCharacterSlots() {
	return (CharacterSlots == 0) ? GetServer()->GetCharacterSlotsPerAccount() : CharacterSlots;
}

void Client::SetPendingZone(uint32_t char_id, uint32_t zone_id, uint32_t instance_id) {
	pending_character = char_id;
	pending_zone = zone_id;
	pending_instance = instance_id;
}

void Client::ReadVersionPacket(const unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) {
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
	p.HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
}