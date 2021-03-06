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
	pending_access_code = 0;
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
		//r->AvailableStartingCities = 63;
		r->Unknown11 = 7;
		r->SubscriptionLevel = 2;
		r->RaceFlag = GetAllowedRaces();
		r->ClassFlag = GetAllowedClasses();

		if (GetVersion() >= 67650) {
			//1 << 59 bit is prison server flag enabled
			static const unsigned char unk4Bytes[] = { 0x7F, 0xFF, 0xF8, 0x90, 0x44, 0x94, 0x00, 0x10, 0x10 };
			static const unsigned char unk7Bytes[] = { 0x7F, 0xFF, 0xFF, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE };

			r->bolUnknown4.assign(reinterpret_cast<const char*>(unk4Bytes), 9);
			r->bolUnknown7.assign(reinterpret_cast<const char*>(unk7Bytes), 9);
		}
		else if (GetVersion() >= 60100) {
			r->Unknown4 = 392192;
			//we had this as 2 separate elements, some kind of flags
			r->Unknown7 = 4219469759ull | (8388607ull << 32);
			//r->RaceUnknown = 255;
		}
		else if (GetVersion() >= 1096) {
			r->Unknown4 = 293888;
			r->Unknown7 = 2145009599ull;
		}
		else if (GetVersion() >= 843) {
			r->Unknown4 = 0xFFFFFFFE;
			//Disable play the fae, trial of the isle, freeplay account, freeplay exceeded
			r->Unknown4 ^= 1 << 1;
			r->Unknown4 ^= 1 << 2;
			r->Unknown4 ^= 1 << 17;
			r->Unknown4 ^= 1 << 18;
			r->Unknown7 = r->Unknown4;
		}

		// Equipment shown during normal character creation
		WorldServer* s = GetServer();
		std::map<uint8_t, std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem> >::iterator itr;
		std::vector<OP_LoginReplyMsg_Packet::ClassItem::StartingItem>::iterator itr2;
		r->bUsePacketClassItems = true;
		for (itr = s->NormalEquipment.begin(); itr != s->NormalEquipment.end(); itr++) {
			OP_LoginReplyMsg_Packet::ClassItem ci;
			ci.ClassID = itr->first;
			for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
				ci.StartingItems.push_back(*itr2);

			r->ClassItems.push_back(ci);
		}

		// Equipment shown during heroic (lvl 90) character creation
		r->bHasHeroicItems = true;
		for (itr = s->HeroicEquipment.begin(); itr != s->HeroicEquipment.end(); itr++) {
			OP_LoginReplyMsg_Packet::ClassItem ci;
			ci.ClassID = itr->first;
			for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
				ci.StartingItems.push_back(*itr2);

			r->HeroicClassItems.push_back(ci);
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
	Bytef buf[4096] = { 0 };
	
	z_stream zstream;
	int zerror = 0;
	zstream.next_in = (Bytef*)log.c_str();
	zstream.avail_in = size;
	zstream.next_out = buf;
	zstream.avail_out = sizeof(buf);
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.opaque = Z_NULL;

	zerror = inflateInit(&zstream);
	if (zerror != Z_OK) {
		return;
	}

	string unpackedMessage;

	for (;;) {
		zerror = inflate(&zstream, 0);
		bool bEnd = (zerror == Z_STREAM_END);
		if (zerror != Z_OK && !bEnd) {
			inflateEnd(&zstream);
			return;
		}

		size_t bytesGenerated = sizeof(buf) - zstream.avail_out;

		if (bytesGenerated == 0) {
			break;
		}

		unpackedMessage.append(reinterpret_cast<const char*>(buf), bytesGenerated);

		if (bEnd) {
			break;
		}

		zstream.avail_out = sizeof(buf);
		zstream.next_out = buf;
	}

	inflateEnd(&zstream);

	LogDebug(LOG_PACKET, 0, "%s : %s", type.c_str(), unpackedMessage.c_str());

	database.SaveClientLog(type, unpackedMessage, GetVersion());
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
	uint32_t struct_version = OP_LoginRequestMsg_Packet::DetermineStructVersion(data, size, offset);
	//We want to handle this packet now because other packets rely on the version set from it
	OP_LoginRequestMsg_Packet p(struct_version);


	p.Read(data, offset, size);
	p.HandlePacket(std::static_pointer_cast<Client>(shared_from_this()));
}