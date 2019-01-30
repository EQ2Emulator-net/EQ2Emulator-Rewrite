#include "OpcodeManager.h"

OpcodeManager* g_OpcodeManager = nullptr;

void OpcodeManager::RegisterEmuOpcodeHelper(const char* name, PacketAllocatorBase* allocator, std::type_index t) {
	if (!g_OpcodeManager) {
		//We have to create this dynamically because static initialization does not guarantee ordering
		//So an opcode may try to register itself before our OpcodeManager is actually initialized if it was global
		static OpcodeManager Manager;
		g_OpcodeManager = &Manager;
	}
	g_OpcodeManager->RegisterAllocator(name, allocator, t);
}