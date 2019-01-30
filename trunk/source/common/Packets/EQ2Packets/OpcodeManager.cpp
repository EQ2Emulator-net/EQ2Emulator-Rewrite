#include "OpcodeManager.h"

OpcodeManager* OpcodeManager::GetGlobal() {
	static OpcodeManager Manager;
	return &Manager;
}

void OpcodeManager::RegisterEmuOpcodeHelper(const char* name, PacketAllocatorBase* allocator, std::type_index t) {
	GetGlobal()->RegisterAllocator(name, allocator, t);
}