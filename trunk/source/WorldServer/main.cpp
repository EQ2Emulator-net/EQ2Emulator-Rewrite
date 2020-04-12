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

WorldServer s;
WorldDatabase database;
Classes classes;
ZoneTalk zoneTalk;

int main(int argc, char **argv)
{
	bool success = true;
	bool looping = true; // Need to move this out at some point

	srand(static_cast<unsigned int>(time(nullptr)));

	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-WorldServer");
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);
	LogInfo(LOG_INIT, 0, "Starting %s v%d.%d %s", EQ2_NAME, EQ2_VERSION_MAJOR, EQ2_VERSION_MINOR, EVE_VERSION_PHASE);

	ConfigReader cr(&s, &database, &zoneTalk);
	success = cr.ReadConfig("world-config.xml");

	if (success)
		success = database.Start();

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading opcodes...");
		success = database.LoadOpcodes();
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading server config...");
		success = database.LoadServerConfig(&s);
	}

	if (success)
		success = s.Open();

	if (success)
		success = zoneTalk.Open();

	{
		XmlStructDumper StructDump;
		StructDump.DumpStructsFile("LoginStructs.xml");
	}

	while (success && looping) {
		Timer::SetCurrentTime();

		success = s.Process();

		if (success)
			success = zoneTalk.Process();

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