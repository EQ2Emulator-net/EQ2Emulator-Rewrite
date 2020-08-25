#include "stdafx.h"

#include "OP_LoginByNumRequestMsg_Packet.h"
#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator g_zoneOperator;

void OP_LoginByNumRequestMsg_Packet::HandlePacket(std::shared_ptr<Client> client) {
	LogError(LOG_PACKET, 0, "Client with account id of %u connected", account_id);
	g_zoneOperator.ClientLogIn(client, this);
}