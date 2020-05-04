#include "LuaInterface.h"

class LuaChatFunctions : public LuaInterface {
	friend class LuaFunctions;
private:
	static void RegisterFunctions(lua_State* state);

	static int Emu_Lua_Say(lua_State* state);
	static int Emu_Lua_CreateConversation(lua_State* state);
	static int Emu_Lua_AddConversationOption(lua_State* state);
	static int Emu_Lua_CloseConversation(lua_State* state);
	static int Emu_Lua_StartConversation(lua_State* state);
	static int Emu_Lua_StartDialogConversation(lua_State* state);
};