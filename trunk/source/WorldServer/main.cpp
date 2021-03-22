#include "stdafx.h"

#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "../common/util.h"
#include "../common/log.h"
#include "../common/version.h"
#include "../common/timer.h"


#include "WorldServer/WorldServer.h"
#include "Database/WorldDatabase.h"
#include "../common/ConfigReader.h"
#include "../common/Classes.h"
#include "ZoneTalk/ZoneTalk.h"
#include "../common/Packets/XmlStructDumper.h"
#include "../common/Rules.h"
#include "WorldServer/CharacterList.h"

WorldServer g_worldServer;
WorldDatabase database;
Classes classes;
ZoneTalk zoneTalk;
RuleManager g_ruleManager;
CharacterList g_characterList;

int main(int argc, char **argv)
{
	bool success = true;
	bool looping = true; // Need to move this out at some point

	srand(static_cast<unsigned int>(time(nullptr)));

	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-WorldServer");
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);
	LogInfo(LOG_INIT, 0, "Starting %s v%d.%d %s", EQ2_NAME, EQ2_VERSION_MAJOR, EQ2_VERSION_MINOR, EQ2_VERSION_PHASE);

	ConfigReader cr(&g_worldServer, &database, &zoneTalk);
	success = cr.ReadConfig("world-config.xml");

	if (success)
		success = database.Start();

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading opcodes...");
		success = database.LoadOpcodes();
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading rules...");
		database.LoadRules(g_ruleManager);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading server config...");
		success = database.LoadServerConfig(&g_worldServer);
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading character list...");
		success = database.LoadCharacterList(g_characterList);
	}

	if (success)
		success = g_worldServer.Open();

	if (success)
		success = zoneTalk.Open();

	{
		//TODO: Add a rule in for whether to generate this file, or just a console command
		XmlStructDumper StructDump;
		StructDump.DumpStructsFile("LoginStructs.xml");
		StructDump.DumpStructsFile("CommonStructs.xml");
	}

	if (success)
		database.SetServerStartTime();

	Timer statTimer(30000, true);

	while (success && looping) {
		Timer::SetCurrentTime();

		success = g_worldServer.Process();

		if (success)
			success = zoneTalk.Process();

		if (statTimer.Check())
			database.SetServerLastUpdateTime();

		SleepMS(5);
	}

	LogInfo(LOG_GENERAL, 0, "Shutting down...");

	logging = false;
	logging_thread.join();
	LoggingSystem::LogStop();
#if defined(_WIN32)
	printf("Press any key to continue...");
	fgetc(stdin);
#endif
}