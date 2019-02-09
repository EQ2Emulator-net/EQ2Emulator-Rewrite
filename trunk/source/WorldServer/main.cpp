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

WorldDatabase database;
Classes classes;

int main(int argc, char **argv)
{
	bool success = true;
	bool looping = true; // Need to move this out at some point
	WorldServer s;
	
	LogStart();
	LogSetPrefix("EQ2Emu-WorldServer");
	LogInfo(LOG_INIT, 0, "Starting %s v%d.%d %s", EQ2_NAME, EQ2_VERSION_MAJOR, EQ2_VERSION_MINOR, EVE_VERSION_PHASE);

	// TODO: config reader
	ConfigReader cr(&s, &database);
	success = cr.ReadConfig("world-config.xml");

	if (success)
		success = database.Start();

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading opcodes...");
		success = database.LoadOpcodes();
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading server variables...");
		success = database.LoadServerVariables(&s);
	}

	if (success)
		success = s.Open();

	while (success && looping) {
		Timer::SetCurrentTime();

		success = s.Process();
		if (success)
			success = s.ProcessClientWrite();

		if (success)
			success = s.ProcessClients();

		SleepMS(5);
	}

	LogInfo(LOG_GENERAL, 0, "Shutting down...");

	database.Stop();

	LogStop();
#if defined(_WIN32)
	printf("Press any key to continue...");
	fgetc(stdin);
#endif
}