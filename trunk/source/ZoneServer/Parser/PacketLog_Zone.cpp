//This file is for interfacing with ZoneServer specific classes

#include "stdafx.h"
#include "../../common/Parser/PacketLog.h"
#include "../Packets/OP_ClientCmdMsg_Packet.h"

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