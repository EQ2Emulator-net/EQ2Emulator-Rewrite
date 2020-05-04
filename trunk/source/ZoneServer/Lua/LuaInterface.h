#pragma once

#include "LuaUserData.h"
#include <cstdint>

class Spawn;
class ZoneServer;

struct LuaScriptInfo {
	std::string filename;
	std::string text;
};

class EmuLuaState {
private:
	EmuLuaState(lua_State* state, const std::shared_ptr<LuaScriptInfo>& script);
public:
	~EmuLuaState();

	static std::shared_ptr<EmuLuaState> Create(const std::shared_ptr<LuaScriptInfo>& script);

	lua_State* state;
	uint32_t nUsers;
};

class LuaInterface {
public:
	//Primitive types
	static void PushLuaNil(lua_State* state);
	static void PushLuaString(lua_State* state, const char* str);
	static void PushLuaFloat(lua_State* state, float f);
	static void PushLuaUInt32(lua_State* state, uint32_t u);
	static void PushLuaInt32(lua_State* state, int32_t i);
	static void PushLuaBool(lua_State* state, bool b);

	static std::string GetLuaString(lua_State* state, int index);
	static float GetLuaFloat(lua_State* state, int index);
	static uint32_t GetLuaUInt32(lua_State* state, int index);
	static int32_t GetLuaInt32(lua_State* state, int index);
	static bool GetLuaBool(lua_State* state, int index);
		
	//Misc
	static bool IsNoneOrNil(lua_State* state, int index);
	static void LuaError(const char* err);
	static void LuaErrorF(const char* fmt, ...);
	static std::shared_ptr<EmuLuaState> LoadSpawnScript(uint32_t id);
	static std::shared_ptr<EmuLuaState> LoadZoneScript(uint32_t id);
	static void PrintStateError(lua_State* state);

	//User data
	static void PushLuaSpawn(lua_State* state, const std::shared_ptr<Spawn>& spawn);
	static void PushLuaZone(lua_State* state, const std::shared_ptr<ZoneServer>& spawn);
	static LuaConversation* PushLuaConversation(lua_State* state);

	static std::shared_ptr<Spawn> GetLuaSpawn(lua_State* state, int index);
	static std::shared_ptr<ZoneServer> GetLuaZone(lua_State* state, int index);
	static LuaConversation* GetLuaConversation(lua_State* state, int index);
};