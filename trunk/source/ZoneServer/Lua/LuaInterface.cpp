#include "stdafx.h"

#include "LuaInterface.h"

void LuaInterface::PushLuaNil(lua_State* state) {
	Emu_CheckLuaStack(state);
	lua_pushnil(state);
}

void LuaInterface::PushLuaString(lua_State* state, const char* str) {
	Emu_CheckLuaStack(state);
	lua_pushstring(state, str);
}

void LuaInterface::PushLuaFloat(lua_State* state, float f) {
	Emu_CheckLuaStack(state);
	lua_pushnumber(state, f);
}

void LuaInterface::PushLuaUInt32(lua_State* state, uint32_t u) {
	Emu_CheckLuaStack(state);
	lua_pushinteger(state, u);
}

void LuaInterface::PushLuaInt32(lua_State* state, int32_t i) {
	Emu_CheckLuaStack(state);
	lua_pushinteger(state, i);
}

void LuaInterface::PushLuaBool(lua_State* state, bool b) {
	Emu_CheckLuaStack(state);
	lua_pushboolean(state, b);
}

std::string LuaInterface::GetLuaString(lua_State* state, int index) {
	std::string ret;
	if (lua_isstring(state, index)) {
		ret = lua_tostring(state, index);
	}
	return ret;
}

float LuaInterface::GetLuaFloat(lua_State* state, int index) {
	//0 if it fails
	return static_cast<float>(lua_tonumber(state, index));
}

uint32_t LuaInterface::GetLuaUInt32(lua_State* state, int index) {
	//0 if it fails
	return static_cast<uint32_t>(lua_tointeger(state, index));
}

int32_t LuaInterface::GetLuaInt32(lua_State* state, int index) {
	//0 if it fails
	return static_cast<int32_t>(lua_tointeger(state, index));
}

std::shared_ptr<Spawn> LuaInterface::GetLuaSpawn(lua_State* state, int index) {
	std::shared_ptr<Spawn> ret;
	if (LuaSpawn* data = LuaDataFactory<LuaSpawn>::GetUserData(state, index)) {
		ret = data->spawn.lock();
	}
	return ret;
}

bool LuaInterface::GetLuaBool(lua_State* state, int index) {
	return lua_toboolean(state, index) == 1;
}

bool LuaInterface::IsNoneOrNil(lua_State* state, int index) {
	return lua_isnoneornil(state, index);
}

void LuaInterface::PushLuaSpawn(lua_State* state, const std::shared_ptr<Spawn>& spawn) {
	if (!spawn) {
		PushLuaNil(state);
		return;
	}

	LuaSpawn* ls = LuaDataFactory<LuaSpawn>::PushNew(state);
	ls->spawn = spawn;
	return;
}

void LuaInterface::PushLuaZone(lua_State* state, const std::shared_ptr<ZoneServer>& zone) {
	if (!zone) {
		PushLuaNil(state);
		return;
	}

	LuaZone* lz = LuaDataFactory<LuaZone>::PushNew(state);
	lz->zone = zone;
	return;
}

std::shared_ptr<ZoneServer> LuaInterface::GetLuaZone(lua_State* state, int index) {
	std::shared_ptr<ZoneServer> ret;
	if (LuaZone* data = LuaDataFactory<LuaZone>::GetUserData(state, index)) {
		ret = data->zone.lock();
	}
	return ret;
}