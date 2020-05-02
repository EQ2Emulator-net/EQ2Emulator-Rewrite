#include "stdafx.h"

#include "LuaInterface.h"
#include "../../common/log.h"

void LuaInterface::PushLuaNil(lua_State* state) {
	Emu_CheckLuaStack(state, 1);
	lua_pushnil(state);
}

void LuaInterface::PushLuaString(lua_State* state, const char* str) {
	Emu_CheckLuaStack(state, 1);
	lua_pushstring(state, str);
}

void LuaInterface::PushLuaFloat(lua_State* state, float f) {
	Emu_CheckLuaStack(state, 1);
	lua_pushnumber(state, f);
}

void LuaInterface::PushLuaUInt32(lua_State* state, uint32_t u) {
	Emu_CheckLuaStack(state, 1);
	lua_pushinteger(state, u);
}

void LuaInterface::PushLuaInt32(lua_State* state, int32_t i) {
	Emu_CheckLuaStack(state, 1);
	lua_pushinteger(state, i);
}

void LuaInterface::PushLuaBool(lua_State* state, bool b) {
	Emu_CheckLuaStack(state, 1);
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

static int emu_lua_panic(lua_State* state) {
	const char* err = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
	LogError(LOG_LUA, 0, "Lua is panicking! error: %s", err);

	//Break the program so we can find the cause of the panic
	assert(false);
	return 0;
}

EmuLuaState::EmuLuaState(lua_State* in_state, const std::shared_ptr<LuaScriptInfo>& script) : state(in_state), nUsers(0) {
	lua_atpanic(state, emu_lua_panic);
	luaL_openlibs(state);
	lua_checkstack(state, 50);
	LuaInterface::RegisterFunctions(state);
	lua_pushstring(state, script->filename.c_str());
	lua_setglobal(state, "GLuaScriptName");
}

EmuLuaState::~EmuLuaState() {
	lua_close(state);
}

std::shared_ptr<EmuLuaState> EmuLuaState::Create(const std::shared_ptr<LuaScriptInfo>& script) {
	lua_State* state = luaL_newstate();
	if (luaL_dostring(state, script->text.c_str()) != 0) {
		return std::shared_ptr<EmuLuaState>();
	}

	return std::shared_ptr<EmuLuaState>(new EmuLuaState(state, script));
}