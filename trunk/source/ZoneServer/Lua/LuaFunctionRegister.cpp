#include "stdafx.h"

#include "LuaInterface.h"

//Register functions/classes here after they have been written
void LuaInterface::RegisterFunctions(lua_State* state) {
	LuaDataFactory<LuaSpawn>::RegisterLuaClass(state);
	LuaDataFactory<LuaZone>::RegisterLuaClass(state);

	lua_register(state, "Say", Emu_Lua_Say);
}