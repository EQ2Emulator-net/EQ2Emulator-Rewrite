#pragma once

#include <type_traits>
#include <cassert>
#include <map>
#include <string>
#include <memory>

//This is a simple wrapper to enable garbage collection and adding of other functions to Lua types bound to c++ objects

#include "../../Lua/lua.hpp"

//Ensure the Lua stack is large enough to push another element onto
#ifdef EQ2_DEBUG
#define Emu_CheckLuaStack(l) assert(lua_checkstack(l, 1))
#else
#define Emu_CheckLuaStack(l) (lua_checkstack(l, 1))
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
		Emu_CheckLuaStack(state);
		//Use this to get the class name and method table
		static T staticRef;
		//Create a new meta table
		luaL_newmetatable(state, staticRef.GetTypeName());
		//Get the table index
		int metatable_index = lua_gettop(state);

		//Set the key value pair to assign the garbage collector function in Lua for this class
		Emu_CheckLuaStack(state);
		lua_pushstring(state, "__gc");
		Emu_CheckLuaStack(state);
		lua_pushcfunction(state, LuaDataFactory<T>::cpp_lua_gc);
		lua_settable(state, metatable_index);

		//Check for any additional functions for this class
		for (auto& itr : staticRef.GetMethodMap()) {
			Emu_CheckLuaStack(state);
			lua_pushstring(state, itr.first.c_str());
			Emu_CheckLuaStack(state);
			lua_pushcfunction(state, itr.second);
			lua_settable(state, metatable_index);
		}
	}

	static T* PushNew(lua_State* state) {
		Emu_CheckLuaStack(state);
		//Allocate a block in Lua to hold a pointer to our object
		T* obj = new T;
		T** ptr = static_cast<T**>(lua_newuserdata(state, sizeof(T*)));
		*ptr = obj;

		//Get the index of the newly created object on the Lua stack
		int obj_index = lua_gettop(state);
		//Now get our registered metatable for this class
		luaL_getmetatable(state, obj->GetTypeName());
		//Assign this metatable to the object so that we can use garbage collection
		lua_setmetatable(state, obj_index);
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