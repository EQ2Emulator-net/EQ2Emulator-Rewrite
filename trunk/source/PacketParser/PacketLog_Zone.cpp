//This file is for interfacing with ZoneServer specific classes

#include "stdafx.h"
#include "PacketLog.h"
#include "../ZoneServer/Packets/OP_LoginByNumRequestMsg_Packet.h"
#include "../ZoneServer/Packets/OP_ClientCmdMsg_Packet.h"

uint32_t PacketLog::ReadLoginByNumRequest(const unsigned char* data, uint32_t size) {
	uint32_t offset = 0;
	if (data[0] == 0) {
		//Set the offset 1 byte past the opcode
		if (data[1] == 9) offset = 5;
		else offset = 2;
	}
	else offset = 1;

	uint32_t pver = OP_LoginByNumRequestMsg_Packet::DetermineStructVersion(data, offset, size);
	OP_LoginByNumRequestMsg_Packet p(pver);
	p.Read(data, offset, size);
	return static_cast<uint32_t>(p.version);
}

void PacketLog::SortClientCommands() {
	uint16_t opcode = opcodeLookup["OP_ClientCmdMsg"];

	std::map<uint16_t, std::vector<std::pair<uint32_t, std::string> > > mySort;

	OP_ClientCmdMsg_Packet p(logVersion);

	auto f = packets.find(opcode);
	if (f != packets.end()) {
		for (auto& itr : f->second) {
			assert(p.Read(reinterpret_cast<const unsigned char*>(itr.second.c_str()), 0, static_cast<uint32_t>(itr.second.size())));
			uint16_t op = p.GetSubOpcode();
			mySort[op].emplace_back(std::move(itr));
		}

		packets.erase(f);
		
		for (auto& itr : mySort) {
			packets.emplace(itr.first, std::move(itr.second));
		}
	}
}