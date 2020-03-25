#include "stdafx.h"

#include "../common/ConfigReader.h"
#include "../common/util.h"
#include "../common/log.h"
#include "WorldTalk/WorldTalk.h"
#include "Database/ZoneDatabase.h"
#include "../common/timer.h"
#include "../common/Classes.h"
#include "ZoneServer/ZoneOperator.h"

ZoneDatabase database;
Classes classes;
ZoneOperator z;

int main() {
	bool looping = true;
	bool success = true;

	srand(static_cast<unsigned int>(time(nullptr)));

	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-ZoneServer");
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	WorldTalk talk;
	
	ConfigReader cr(&z, &database, &talk);
	success = cr.ReadConfig("zone-config.xml");

	if (success)
		database.Start();

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading opcodes...");
		success = database.LoadOpcodes();
	}

	if (success) {
		LogDebug(LOG_DATABASE, 0, "Loading commands...");
		success = database.LoadCommands();
	}

	if (success)
		success = z.Open();

	if (success)
		success = talk.Open();

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

		success = z.Process();

		SleepMS(5);
	}

	logging = false;
	logging_thread.join();
	LoggingSystem::LogStop();

	return 0;
}