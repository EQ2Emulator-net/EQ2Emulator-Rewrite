#include "stdafx.h"
#include "../ZoneTalk/ZoneStream.h"
#include "../ZoneTalk/ZoneTalk.h"
#include "../WorldServer/WorldServer.h"
#include "../WorldServer/Client.h"
#include "../WorldServer/CharacterList.h"
#include "../WorldServer/Character.h"

//EQ2Packets
#include "../Packets/OP_PlayCharacterRequestMsg_Packet.h"

//Emu Packets
#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServerReply_Packet.h"

#include "../../common/Packets/EmuPackets/Emu_RequestZoneReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_TransferClientConfirm_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ClientLoginConfirm_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_CancelClientTransfer_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ClientSessionEnded_Packet.h"

extern ZoneTalk zoneTalk;
extern WorldServer g_worldServer;
extern CharacterList g_characterList;

void Emu_RegisterZoneServer_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	// TODO: Actually do checks instead of just accepting the ZoneServer
	Emu_RegisterZoneServerReply_Packet* p = new Emu_RegisterZoneServerReply_Packet;
	p->reply = 1;
	p->serverName = g_worldServer.GetName();
	z->QueuePacket(p);

	// Set the ZoneServer info
	z->SetIP(ip);
	z->SetPort(port);
	LogDebug(LOG_NET, 0, "New zone server registered.  ip = %s:%u", ip.c_str(), port);
}

void Emu_RequestZoneReply_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	if (reply > 0) {
		zoneTalk.AddZone(z, reply, 0);
	}
	else {
		// error
	}
}

void Emu_TransferClientConfirm_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	auto client = g_worldServer.GetClientByAccessCode(access_code);
	if (!client) {
		return;
	}

	auto character = g_characterList.GetCharacterByID(characterID);
	
	if (!character) {
		//Shouldn't happen
		return;
	}

	// Send packet to client allowing them to connect to the zone or telling them they can't right now
	OP_PlayCharacterReplyMsg_Packet* p = new OP_PlayCharacterReplyMsg_Packet(client->GetVersion());

	if (character->IsOnline()) {
		//This character is either still marked as online or has a pending connection
		p->response = PlayCharacterResponse::EAccountInUse;
		client->QueuePacket(p);

		auto cancel = new Emu_CancelClientTransfer_Packet;
		cancel->access_code = access_code;
		z->QueuePacket(cancel);
		return;
	}

	g_worldServer.AddPendingClientTransfer(access_code, character, z);
	character->SetPendingZoneConnection();

	// Send packet to client allowing them to connect to the zone
	p->response = PlayCharacterResponse::ESuccess;
	p->server = z->GetIP();
	p->port = z->GetPort();
	p->account_id = client->GetAccountID();
	p->access_code = access_code;
	client->QueuePacket(p);
}

void Emu_ClientLoginConfirm_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	g_worldServer.ConfirmClientTransfer(access_code, sessionID, zoneName);
}

void Emu_ClientSessionEnded_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	auto character = g_characterList.GetCharacterBySessionID(sessionID);
	if (character) {
		character->RemoveZoneStream(sessionID);
	}
}