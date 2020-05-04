#include "LuaInterface.h"

class LuaChatFunctions : public LuaInterface {
	friend class LuaFunctions;
private:
	static void RegisterFunctions(lua_State* state);

	static int Emu_Lua_Say(lua_State* state);
};