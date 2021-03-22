#include "stdafx.h"

#include "OpcodeManager.h"

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
				if (NetDebugEnabled()) {
					LogDebug(LOG_PACKET, 0, "Found opcode: %s", op->second->opName);
				}
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
					packet->opcode = op.first;
					return true;
				}
			}
			LogError(LOG_PACKET, 0, "Could not find an opcode for packet %s, version %u", allocator->opName, version);
			return false;
		}
	}

	LogDebug(LOG_PACKET, 0, "Could not find a version range for version %u", packet->GetVersion());
	return false;
}