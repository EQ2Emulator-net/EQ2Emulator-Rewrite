#include "OpcodeManager.h"

OpcodeManager* OpcodeManager::GetGlobal() {
	static OpcodeManager Manager;
	return &Manager;
}

void OpcodeManager::RegisterEmuOpcodeHelper(const char* name, PacketAllocatorBase* allocator, std::type_index t, 
	const char* outfile, const int32_t* versions, int32_t num_versions) {
	GetGlobal()->RegisterAllocator(name, allocator, t, outfile, versions, num_versions);
}