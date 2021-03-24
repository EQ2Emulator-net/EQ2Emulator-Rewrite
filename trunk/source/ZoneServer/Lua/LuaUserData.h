#pragma once

#include <type_traits>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "../../common/EmuAssert.h"

//This is a simple wrapper to enable garbage collection and adding of other functions to Lua types bound to c++ objects

#ifdef _WIN32
#include "../../Lua/lua.hpp"
#else
#include "../../Lua/lua.h"
#include "../../Lua/lualib.h"
#include "../../Lua/lauxlib.h"
#endif

//Ensure the Lua stack is large enough to push another element onto
#ifdef EQ2_DEBUG
#define Emu_CheckLuaStack(l, n) EmuAssert(lua_checkstack((l), (n)))
#else
#define Emu_CheckLuaStack(l, n) (lua_checkstack((l), (n)))
#endif

class LuaUserData {
protected:
	LuaUserData() = default;
	
public:
	virtual ~LuaUserData() = default;

	virtual const char* GetTypeName() = 0;
	virtual const std::map<std::string, lua_CFunction>& GetMethodMap() = 0;
};

//This template is only to be used with classes that derive from LuaUserData!
template <typename T> class LuaDataFactory {
	static_assert(std::is_base_of<LuaUserData, T>::value);
public:
	static void RegisterLuaClass(lua_State* state) {
		Emu_CheckLuaStack(state, 3);
		//Use this to get the class name and method table
		static T staticRef;
		static const char* clsName = staticRef.GetTypeName();
		static const std::map<std::string, lua_CFunction>& methodMap = staticRef.GetMethodMap();

		//Create a new meta table
		if (luaL_newmetatable(state, clsName) == 1) {
			//Get the table index
			int metatable_index = lua_gettop(state);

			//Set the key value pair to assign the garbage collector function in Lua for this class
			lua_pushstring(state, "__gc");
			lua_pushcfunction(state, LuaDataFactory<T>::cpp_lua_gc);
			lua_settable(state, metatable_index);

			//Check for any additional functions for this class
			for (auto& itr : methodMap) {
				lua_pushstring(state, itr.first.c_str());
				lua_pushcfunction(state, itr.second);
				lua_settable(state, metatable_index);
			}
		}
		//Pop the table off the stack
		lua_pop(state, 1);
	}

	static T* PushNew(lua_State* state) {
		Emu_CheckLuaStack(state, 2);
		//Allocate a block in Lua to hold a pointer to our object
		T* obj = new T;
		T** ptr = static_cast<T**>(lua_newuserdata(state, sizeof(T*)));
		*ptr = obj;

		//Assign this metatable to the object so that we can use garbage collection
		luaL_setmetatable(state, obj->GetTypeName());
		return obj;
	}

	static int cpp_lua_gc(lua_State* state) {
		T** obj = static_cast<T**>(lua_touserdata(state, -1));
		delete *obj;
		return 0;
	}

	static T* GetUserData(lua_State* state, int index) {
		T* ret = nullptr;
		if (lua_isuserdata(state, index)) {
			LuaUserData* data = *static_cast<LuaUserData**>(lua_touserdata(state, index));
			ret = dynamic_cast<T*>(data);
		}
		return ret;
	}
};

class LuaSpawn : public LuaUserData {
public:
	LuaSpawn() = default;
	~LuaSpawn() = default;

	std::weak_ptr<class Spawn> spawn;

	const char* GetTypeName() override {
		return "Spawn";
	}

	static int LuaEqOverride(lua_State* state);

	const std::map<std::string, lua_CFunction>& GetMethodMap();
};

class LuaZone : public LuaUserData {
public:
	LuaZone() = default;
	~LuaZone() = default;

	std::weak_ptr<class ZoneServer> zone;

	const char* GetTypeName() override {
		return "Zone";
	}

	static int LuaEqOverride(lua_State * state);

	const std::map<std::string, lua_CFunction>& GetMethodMap();
};

struct ConversationOption {
	std::string option;
	std::string function;
};

class LuaConversation : public LuaUserData {
public:
	LuaConversation() = default;
	~LuaConversation() = default;

	std::vector<ConversationOption> options;

	const char* GetTypeName() override {
		return "Conversation";
	}

	const std::map<std::string, lua_CFunction>& GetMethodMap();
};