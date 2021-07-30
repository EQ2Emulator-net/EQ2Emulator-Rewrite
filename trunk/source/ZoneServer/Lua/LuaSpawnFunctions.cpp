#include "stdafx.h"

#include "LuaSpawnFunctions.h"
#include "../Spawns/Entity.h"
#include "../AI/MovementLocationInfo.h"
#include "../Controllers/NPCController.h"

void LuaSpawnFunctions::RegisterFunctions(lua_State* state) {
	lua_register(state, "FaceTarget", Emu_Lua_FaceTarget);
	lua_register(state, "GetRace", Emu_Lua_GetRace);
	lua_register(state, "GetName", Emu_Lua_GetName);
	lua_register(state, "MovementLoopAddLocation", Emu_Lua_MovementLoopAddLocation);
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

int LuaSpawnFunctions::Emu_Lua_GetRace(lua_State* state) {
	auto spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		LuaError("Lua GetRace command error: spawn is invalid");
		return 0;
	}

	PushLuaUInt32(state, spawn->GetRace());
	return 1;
}

int LuaSpawnFunctions::Emu_Lua_GetName(lua_State* state) {
	auto spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		LuaError("Lua GetName command error: spawn is invalid");
		return 0;
	}

	PushLuaString(state, spawn->GetName().c_str());
	return 1;
}

int LuaSpawnFunctions::Emu_Lua_MovementLoopAddLocation(lua_State* state) {
	std::shared_ptr<Spawn> spawn = GetLuaSpawn(state, 1);
	if (!spawn) {
		LuaError("Lua MovementLoopAddLocation command error: spawn is not valid");
		return 0;
	}

	std::shared_ptr<NPCController> controller = std::dynamic_pointer_cast<NPCController>(spawn->GetController());
	if (!controller) {
		LuaError("Lua MovementLoopAddLocation command error: unable to get a controller for the given spawn");
		return 0;
	}

	std::shared_ptr<MovementLocationInfo> loc = std::make_shared<MovementLocationInfo>();
	loc->x = GetLuaFloat(state, 2);
	loc->y = GetLuaFloat(state, 3);
	loc->z = GetLuaFloat(state, 4);
	float speed = GetLuaFloat(state, 5);
	loc->speed = speed == 0 ? 2 : speed;
	loc->delay = GetLuaFloat(state, 6);
	loc->callback = GetLuaString(state, 7);

	// This will probably need to be reworked when AI is figured out
	controller->AddMovementLocation(loc);
	return 1;
}