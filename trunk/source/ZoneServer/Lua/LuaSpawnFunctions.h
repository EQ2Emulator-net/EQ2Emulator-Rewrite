#include "LuaInterface.h"

class LuaSpawnFunctions : public LuaInterface {
	friend class LuaFunctions;
private:
	static void RegisterFunctions(lua_State* state);
};