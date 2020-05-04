#pragma once

#include <memory>
#include <map>
#include <string>
#include "../../common/Mutex.h"

class DatabaseResult;
struct LuaScriptInfo;

enum class EScriptType {
	ESpawnScript,
	EZoneScript,
	ESpellScript,
	EQuestScript,
	EItemScript
};

class LuaGlobals {
public:
	LuaGlobals() = default;
	~LuaGlobals() = default;

	std::shared_ptr<LuaScriptInfo> GetSpawnScript(uint32_t id);
	std::shared_ptr<LuaScriptInfo> GetZoneScript(uint32_t id);

	void ProcessSpawnScriptResult(DatabaseResult& result);
	void ProcessZoneScriptResult(DatabaseResult& result);

private:
	uint32_t ProcessDBResult(DatabaseResult& result, EScriptType scriptType);
	std::shared_ptr<LuaScriptInfo> GetScript(uint32_t id, EScriptType scriptType);

	std::map<EScriptType, std::map<uint32_t, std::shared_ptr<LuaScriptInfo> > > m_scriptTextCache;
	Mutex m_scriptTextCache_lock;
};