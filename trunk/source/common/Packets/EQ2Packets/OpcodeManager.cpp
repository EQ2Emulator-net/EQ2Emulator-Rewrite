#include "stdafx.h"

#include "OpcodeManager.h"

#ifdef EQ2_ZONE
#include "../../../ZoneServer/Packets/OP_ClientCmdMsg_Packet.h"
#endif

OpcodeManager* OpcodeManager::GetGlobal() {
	static OpcodeManager Manager;
	return &Manager;
}

void OpcodeManager::RegisterEQ2OpcodeHelper(const char* name, EQ2PacketAllocatorBase* allocator, std::type_index t,
	const char* outfile, const uint32_t* versions, int32_t num_versions) {
	GetGlobal()->RegisterAllocator(name, allocator, t, outfile, versions, num_versions);
}

EQ2Packet* OpcodeManager::GetPacketForVersion(uint32_t version, uint16_t opcode) {
	EQ2Packet* ret = nullptr;
	for (auto& itr : versions) {
		versionRange_t range = itr.first;
		if (range.first <= version && range.second >= version) {
			auto op = itr.second.find(opcode);
			if (op != itr.second.end()) {
				ret = op->second->Create(version);
				ret->opcode = opcode;
			}
			break;
		}
	}

	return ret;
}

bool OpcodeManager::SetOpcodeForPacket(EQ2Packet* packet) {
	auto itr = type_map.find(typeid(*packet));
	assert(("Please register this packet class with an opcode.", itr != type_map.end()));

	EQ2PacketAllocatorBase* allocator = itr->second;
	uint32_t version = packet->GetVersion();
	for (auto& itr : versions) {
		versionRange_t range = itr.first;
		if (range.first <= version && range.second >= version) {
			for (auto& op : itr.second) {
				if (op.second == allocator) {
#ifdef EQ2_ZONE
					if (auto ccmsg = dynamic_cast<OP_ClientCmdMsg_Packet*>(packet)) {
						if (typeid(*ccmsg) != typeid(OP_ClientCmdMsg_Packet)) {
							OP_ClientCmdMsg_Packet ccmd(version);
							SetOpcodeForPacket(&ccmd);
							packet->opcode = ccmd.opcode;
							ccmsg->SetSubOpcode(op.first);
							return true;
						}
					}
#endif
					packet->opcode = op.first;
					return true;
				}
			}
			assert(("Could not find an opcode for this packet! Check it out.", false));
			return false;
		}
	}

	LogDebug(LOG_PACKET, 0, "Could not find a version range for version %u", packet->GetVersion());
	return false;
}