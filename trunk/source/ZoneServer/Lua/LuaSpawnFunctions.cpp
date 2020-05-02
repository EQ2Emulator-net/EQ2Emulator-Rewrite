#include "stdafx.h"

#include "LuaInterface.h"
#include "../Spawns/Spawn.h"
#include "../ZoneServer/ZoneServer.h"

int LuaInterface::Emu_Lua_Say(lua_State* state) {
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