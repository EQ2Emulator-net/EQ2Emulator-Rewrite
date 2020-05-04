#include "stdafx.h"

#include "LuaChatFunctions.h"
#include "../Spawns/Spawn.h"
#include "../ZoneServer/ZoneServer.h"

void LuaChatFunctions::RegisterFunctions(lua_State* state) {
	lua_register(state, "Say", Emu_Lua_Say);
}

int LuaChatFunctions::Emu_Lua_Say(lua_State* state) {
	auto spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		return 0;
	}

	auto zone = spawn->GetZone();
	if (!zone) {
		return 0;
	}

	std::string text = GetLuaString(state, 2);

	zone->chat.HandleSay(text.c_str(), spawn);
	return 0;
}