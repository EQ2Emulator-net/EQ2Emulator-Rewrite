#include "stdafx.h"

#include "LuaGlobals.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/database-result.h"
#include "../../common/util.h"
#include "LuaInterface.h"
#include "../../common/log.h"

bool ZoneDatabase::LoadSpawnScripts(LuaGlobals& globals) {
	DatabaseResult result;
	bool success = Select(&result, "SELECT id, `name` FROM lua_scripts WHERE `type` = 'Spawn'");

	if (!success) {
		return false;
	}

	globals.ProcessSpawnScriptResult(result);

	return success;
}

void LuaGlobals::ProcessSpawnScriptResult(DatabaseResult& result) {
	uint32_t count = ProcessDBResult(result, EScriptType::ESpawnScript);
	LogInfo(LOG_LUA, 0, "Loaded %u spawn scripts.", count);
}

bool ZoneDatabase::LoadZoneScripts(LuaGlobals& globals) {
	DatabaseResult result;
	bool success = Select(&result, "SELECT id, `name` FROM lua_scripts WHERE `type` = 'Zone'");

	if (!success) {
		return false;
	}

	globals.ProcessZoneScriptResult(result);

	return success;
}

void LuaGlobals::ProcessZoneScriptResult(DatabaseResult& result) {
	uint32_t count = ProcessDBResult(result, EScriptType::EZoneScript);
	LogInfo(LOG_LUA, 0, "Loaded %u zone scripts.", count);
}

uint32_t LuaGlobals::ProcessDBResult(DatabaseResult& result, EScriptType scriptType) {
	uint32_t count = 0;

	std::map<uint32_t, std::shared_ptr<LuaScriptInfo> > scripts;

	lua_State* tmpState = luaL_newstate();
	luaL_openlibs(tmpState);

	while (result.Next()) {
		auto script = std::make_shared<LuaScriptInfo>();
		uint32_t id = result.GetUInt32(0);
		script->filename = result.GetString(1);

		if (!GetFileAsString(script->text, script->filename.c_str())) {
			LuaInterface::LuaErrorF("Could not open the script '%s'!", script->filename.c_str());
			continue;
		}

		if (luaL_dostring(tmpState, script->text.c_str()) != LUA_OK) {
			LuaInterface::LuaErrorF("Error in the script: %s:\n%s", script->filename.c_str(), lua_tostring(tmpState, -1));
			continue;
		}

		scripts[id] = script;

		count++;
	}

	lua_close(tmpState);

	WriteLocker lock(m_scriptTextCache_lock);
	m_scriptTextCache[scriptType] = std::move(scripts);

	return count;
}

std::shared_ptr<LuaScriptInfo> LuaGlobals::GetScript(uint32_t id, EScriptType scriptType) {
	ReadLocker lock(m_scriptTextCache_lock);
	auto itr = m_scriptTextCache.find(scriptType);
	if (itr != m_scriptTextCache.end()) {
		auto script = itr->second.find(id);
		if (script != itr->second.end()) {
			return script->second;
		}
	}

	return std::shared_ptr<LuaScriptInfo>();
}

std::shared_ptr<LuaScriptInfo> LuaGlobals::GetSpawnScript(uint32_t id) {
	return GetScript(id, EScriptType::ESpawnScript);
}

std::shared_ptr<LuaScriptInfo> LuaGlobals::GetZoneScript(uint32_t id) {
	return GetScript(id, EScriptType::EZoneScript);
}