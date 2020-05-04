#include "stdafx.h"

#include "LuaSpawnFunctions.h"
#include "../Spawns/Entity.h"

void LuaSpawnFunctions::RegisterFunctions(lua_State* state) {
	lua_register(state, "FaceTarget", Emu_Lua_FaceTarget);
}

int LuaSpawnFunctions::Emu_Lua_FaceTarget(lua_State* state) {
	auto spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		LuaError("Lua FaceTarget command error: spawn is invalid");
		return 0;
	}

	auto faceTarget = GetLuaSpawn(state, 2);
	if (!faceTarget) {
		LuaError("Lua FaceTarget command error: target to face is invalid");
		return 0;
	}

	spawn->FaceSpawn(faceTarget);
	return 0;
}