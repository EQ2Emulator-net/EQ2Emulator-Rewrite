#include "stdafx.h"

#include "LuaUserData.h"
#include "LuaInterface.h"

int LuaSpawn::LuaEqOverride(lua_State* state) {
	auto s1 = LuaInterface::GetLuaSpawn(state, 1);
	auto s2 = LuaInterface::GetLuaSpawn(state, 2);
	LuaInterface::PushLuaBool(state, s1 == s2);
	return 1;
}

const std::map<std::string, lua_CFunction>& LuaSpawn::GetMethodMap() {
	struct MyFunctionMap : public std::map<std::string, lua_CFunction> {
		MyFunctionMap() {
			emplace("__eq", LuaEqOverride);
		}
	};

	static const MyFunctionMap myMap;
	return myMap;
}

int LuaZone::LuaEqOverride(lua_State* state) {
	auto z1 = LuaInterface::GetLuaZone(state, 1);
	auto z2 = LuaInterface::GetLuaZone(state, 2);
	LuaInterface::PushLuaBool(state, z1 == z2);
	return 1;
}

const std::map<std::string, lua_CFunction>& LuaZone::GetMethodMap() {
	struct MyFunctionMap : public std::map<std::string, lua_CFunction> {
		MyFunctionMap() {
			emplace("__eq", LuaEqOverride);
		}
	};

	static const MyFunctionMap myMap;
	return myMap;
}

const std::map<std::string, lua_CFunction>& LuaConversation::GetMethodMap() {
	struct MyFunctionMap : public std::map<std::string, lua_CFunction> {
		MyFunctionMap() {
		}
	};

	static const MyFunctionMap myMap;
	return myMap;
}