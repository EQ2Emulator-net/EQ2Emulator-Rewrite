#include "stdafx.h"
#include "../WorldTalk/WorldStream.h"
#include "../WorldTalk/WorldTalk.h"

#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServerReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZone_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZoneReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_TransferClient_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_TransferClientConfirm_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_CancelClientTransfer_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ZoneTransferReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ItemIDReply_Packet.h"

#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/ZoneOperator.h"
#include "../Items/MasterItemList.h"

extern ZoneOperator g_zoneOperator;
extern MasterItemList g_masterItemList;

void Emu_RegisterZoneServerReply_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	if (reply == 1) {
		w->SetAuthentication(EAuthentication::EAuthGranted);
		g_zoneOperator.SetWorldStream(w);
		g_zoneOperator.SetWorldServerName(serverName);
		LogDebug(LOG_NET, 0, "Zoneserver is now authenticated.");

		static bool bItemIDRequestSent = false;
		if (!bItemIDRequestSent) {
			g_masterItemList.SendIDRequestToWorld();
			bItemIDRequestSent = true;
		}
	}
	else {
		w->SetAuthentication(EAuthentication::EAuthDenied);
		LogDebug(LOG_NET, 0, "World server rejected this zoneserver.");
	}
}

void Emu_RequestZone_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	Emu_RequestZoneReply_Packet* p = new Emu_RequestZoneReply_Packet();

	if (zone_id == 0) {
		LogError(LOG_ZONE, 0, "Zone request packet received with 0 for the zone id.");	
		p->reply = 0;
		w->QueuePacket(p);
		return;
	}

	std::shared_ptr<ZoneServer> zone = g_zoneOperator.AddNewZone(zone_id, instance_id);
	if (zone) {
		p->reply = zone->GetID();
		w->QueuePacket(p);
	}
	else {
		p->reply = 0;
		w->QueuePacket(p);
	}
}

void Emu_TransferClient_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	PendingClient pc;
	pc.access_code = access_code;
	pc.character_id = character_id;
	pc.zone_id = zone_id;
	pc.instance_id = instance_id;

	g_zoneOperator.AddPendingClient(account_id, pc);

	auto p = new Emu_TransferClientConfirm_Packet;
	p->access_code = access_code;
	p->characterID = character_id;
	p->bFromZone = bFromZone;
	w->QueuePacket(p);
}

void Emu_CancelClientTransfer_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	g_zoneOperator.RemovePendingClient(access_code);
}

void Emu_ZoneTransferReply_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	g_zoneOperator.HandleZoneTransferReply(this);
}

void Emu_ItemIDReply_Packet::HandlePacket(std::shared_ptr<WorldStream> ws) {
	if (rangeLow == 0 && rangeHigh == 0) {
		LogError(LOG_ITEM, 0, "World server returned an error trying to obtain an item unique ID range!");
		return;
	}
	g_masterItemList.AddUniqueIDRange(rangeLow, rangeHigh);
}