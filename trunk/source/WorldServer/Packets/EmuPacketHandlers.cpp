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
#include "../../common/Packets/EmuPackets/Emu_RequestZoneTransfer_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ZoneTransferReply_Packet.h"

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
	auto character = g_characterList.GetCharacterByID(characterID);
	
	if (!character) {
		//Shouldn't happen
		return;
	}

	if (bFromZone) {
		ZoneTalk::PendingSessionTransfer transfer = zoneTalk.PopPendingSessionTransfer(access_code);

		auto zs = transfer.zs.lock();

		if (!zs || transfer.sessionID == 0) {
			//shouldn't happen
			return;
		}

		g_worldServer.AddPendingClientTransfer(access_code, character, z);
		character->SetPendingZoneConnection();

		//Let zone know what server to forward their client to
		auto p = new Emu_ZoneTransferReply_Packet;
		p->access_code = access_code;
		p->host = z->GetIPString();
		p->port = z->GetPort();
		p->sessionID = transfer.sessionID;
		zs->QueuePacket(p);
	}
	else {
		auto client = g_worldServer.GetClientByAccessCode(access_code);
		if (!client) {
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

//This packet is a request from a zoneserver to transfer one of their clients to another zone
void Emu_RequestZoneTransfer_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	auto zs = zoneTalk.GetAvailableZone(zoneID);
	if (zs) {
		//The zone that the client is trying to transfer to is already running, forward the request
		uint32_t access_code = zoneTalk.TransferClientToZone(zs, characterID, zoneID, accountID, instanceID, true);
		zoneTalk.AddPendingSessionTransfer(access_code, sessionID, z);
	}
	else if (!zoneTalk.HasPendingZone(zoneID)) {
		//We could not request this zone be started for some reason, let zone know there is a problem (shouldn't happen tbh)
		auto p = new Emu_ZoneTransferReply_Packet;
		p->bError = true;
		p->sessionID = sessionID;
		z->QueuePacket(p);
	}
	else {
		//We have to wait for the zone to start up, we'll request the transfer after that happens
		zoneTalk.AddPendingZoneTransfer(characterID, zoneID, sessionID, accountID);
	}
}