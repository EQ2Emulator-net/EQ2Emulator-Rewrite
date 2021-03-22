#include "stdafx.h"

#include "OP_LoginRequestMsg_Packet.h"

void OP_LoginRequestMsg_Packet::HandlePacket(std::shared_ptr<Client> client) {
	client->SetVersion((uint32_t)Version);

	if (Password == "")
		Password = AccessCode;

	client->LogIn(Username, Password);
}