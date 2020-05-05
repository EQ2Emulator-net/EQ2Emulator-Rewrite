#include "LuaInterface.h"

class LuaSpawnFunctions : public LuaInterface {
	friend class LuaFunctions;
private:
	static void RegisterFunctions(lua_State* state);

	static int Emu_Lua_FaceTarget(lua_State* state);
	static int Emu_Lua_GetRace(lua_State* state);
	static int Emu_Lua_GetName(lua_State* state);
};