#include "stdafx.h"

#include "LuaFunctions.h"
#include "LuaChatFunctions.h"
#include "LuaSpawnFunctions.h"

//Register functions/classes here after they have been written
void LuaFunctions::RegisterFunctions(lua_State* state) {
	//Class registration
	LuaDataFactory<LuaSpawn>::RegisterLuaClass(state);
	LuaDataFactory<LuaZone>::RegisterLuaClass(state);
	LuaDataFactory<LuaConversation>::RegisterLuaClass(state);

	//Function registration
	LuaChatFunctions::RegisterFunctions(state);
	LuaSpawnFunctions::RegisterFunctions(state);
}