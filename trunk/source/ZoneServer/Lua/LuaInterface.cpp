#include "stdafx.h"

#include "LuaInterface.h"
#include "LuaGlobals.h"
#include "../../common/log.h"
#include "LuaFunctions.h"
#include "../../common/EmuAssert.h"

extern LuaGlobals g_luaGlobals;

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

LuaConversation* LuaInterface::PushLuaConversation(lua_State* state) {
	return LuaDataFactory<LuaConversation>::PushNew(state);
}

LuaConversation* LuaInterface::GetLuaConversation(lua_State* state, int index) {
	return LuaDataFactory<LuaConversation>::GetUserData(state, index);
}

static int emu_lua_panic(lua_State* state) {
	const char* err = lua_isstring(state, -1) ? lua_tostring(state, -1) : "";
	LogError(LOG_LUA, 0, "Lua is panicking! error: %s", err);

	//Break the program so we can find the cause of the panic
	EmuAssert(false);
	return 0;
}

EmuLuaState::EmuLuaState(lua_State* in_state, const std::shared_ptr<LuaScriptInfo>& script) : state(in_state), nUsers(0) {
	lua_atpanic(state, emu_lua_panic);
	luaL_openlibs(state);
	lua_checkstack(state, 50);
	LuaFunctions::RegisterFunctions(state);
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

void LuaInterface::LuaError(const char* err) {
	LogError(LOG_LUA, 0, err);

	//TODO: Push errors to in-game chat
}

void LuaInterface::LuaErrorF(const char* fmt, ...) {
	char buf[1024];

	va_list argptr;

	va_start(argptr, fmt);
	int size;
	if ((size = vsnprintf(buf, sizeof(buf), fmt, argptr)) >= sizeof(buf)) {
		std::unique_ptr<char[]> dynamicBuf;
		//This message will not fit in our stack buf, allocate a larger one
		dynamicBuf.reset(new char[size + 1]);
		vsnprintf(dynamicBuf.get(), size + 1, fmt, argptr);
		LuaError(dynamicBuf.get());
	}
	else {
		LuaError(buf);
	}
	va_end(argptr);
}

std::shared_ptr<EmuLuaState> LuaInterface::LoadSpawnScript(uint32_t id) {
	std::shared_ptr<EmuLuaState> ret;

	auto scriptInfo = g_luaGlobals.GetSpawnScript(id);

	if (!scriptInfo) {
		return ret;
	}

	return EmuLuaState::Create(scriptInfo);
}

std::shared_ptr<EmuLuaState> LuaInterface::LoadZoneScript(uint32_t id) {
	std::shared_ptr<EmuLuaState> ret;

	auto scriptInfo = g_luaGlobals.GetZoneScript(id);

	if (!scriptInfo) {
		return ret;
	}

	return EmuLuaState::Create(scriptInfo);
}

void LuaInterface::PrintStateError(lua_State* state) {
	std::string err = lua_tostring(state, -1);
	lua_getglobal(state, "GLuaScriptName");
	LuaErrorF("Lua script error in script %s:\n%s", lua_tostring(state, lua_gettop(state)), err.c_str());
	lua_pop(state, 1);
}