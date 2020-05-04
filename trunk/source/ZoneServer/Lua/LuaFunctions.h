#include "LuaInterface.h"

class LuaFunctions : public LuaInterface {
public:
	static void RegisterFunctions(lua_State* state);
};