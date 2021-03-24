#include "stdafx.h"

#include "../common/ConfigReader.h"
#include "../common/util.h"
#include "../common/log.h"
#include "WorldTalk/WorldTalk.h"
#include "Database/ZoneDatabase.h"
#include "../common/timer.h"
#include "../common/Classes.h"
#include "ZoneServer/ZoneOperator.h"
#include "Commands/CommandProcess.h"
#include "../common/Packets/XmlStructDumper.h"
#include "../common/Rules.h"
#include "Chat/ChatFilterLookup.h"
#include "Spawns/EntityCommands.h"
#include "ZoneServer/MasterZoneLookup.h"
#include "Lua/LuaGlobals.h"
#include "Items/MasterItemList.h"
#include "Parser/ParserZone.h"

ZoneDatabase database;
Classes classes;
ZoneOperator g_zoneOperator;
CommandProcess g_commandProcess;
RuleManager g_ruleManager;
ChatFilterLookup g_chatFilterLookup;
MasterEntityCommandList g_masterEntityCommandList;
MasterZoneLookup g_masterZoneLookup;
LuaGlobals g_luaGlobals;
MasterItemList g_masterItemList;

int main(int argc, char** argv) {
	bool looping = true;
	bool success = true;

	srand(static_cast<unsigned int>(time(nullptr)));

	WorldTalk talk;

	std::cin.get();

	{
		ConfigReader cr(&g_zoneOperator, &database, &talk);
		success = cr.ReadConfig("zone-config.xml");
	}

	if (success)
		database.Start();

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading opcodes...");
		success = database.LoadOpcodes();
	}

	if (argc > 1 && strcmp(argv[1], "--parse") == 0) {
		//Transfer control to the parser
		ParserZone(argc, argv);
		return 0;
	}

	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-ZoneServer");
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	std::vector<std::thread> loadingThreads;

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading rules...");
		database.LoadRules(g_ruleManager);
	}

	if (success) {
		//These take awhile so load them asyncronously
		auto LoadItemsHelper = []{
			database.LoadMasterItems(g_masterItemList);
		};
		loadingThreads.emplace_back(LoadItemsHelper);

		LogDebug(LOG_DATABASE, 0, "Loading spawn scripts...");
		auto LoadSpawnScriptsHelper = [] {
			database.LoadSpawnScripts(g_luaGlobals);
		};
		loadingThreads.emplace_back(LoadSpawnScriptsHelper);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading commands...");
		success = database.LoadCommands(g_commandProcess);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading chat filters...");
		success = database.LoadChatFilters(g_chatFilterLookup);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading entity commands...");
		success = database.LoadEntityCommands(g_masterEntityCommandList);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading master zone list...");
		success = database.LoadMasterZoneList(g_masterZoneLookup);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading zone scripts...");
		success = database.LoadZoneScripts(g_luaGlobals);
	}

	//Wait on our child loading threads
	for (auto& itr : loadingThreads) {
		itr.join();
	}
	loadingThreads.clear();

	if (success)
		success = g_zoneOperator.Open();

	if (success)
		success = talk.Open();		

	{
		//TODO: Add a rule in for whether to generate this file
		XmlStructDumper structDump;
		structDump.DumpStructsFile("ZoneStructs.xml");
	}

	Timer reconnectTimer;

	while (looping && success) {
		Timer::SetCurrentTime();

		if (!talk.Process()) {
			if (!reconnectTimer.Enabled()) {
				reconnectTimer.Start(10000);
				LogError(LOG_NET, 0, "Lost connection to the WorldServer. Attempting reconnection.");
			}
			else if (reconnectTimer.Check()) {
				if (talk.Open()) {
					reconnectTimer.Disable();
				}
			}
		}

		success = g_zoneOperator.Process();

		SleepMS(5);
	}

	logging = false;
	logging_thread.join();
	LoggingSystem::LogStop();

	return 0;
}