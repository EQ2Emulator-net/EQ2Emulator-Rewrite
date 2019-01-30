#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "../common/util.h"
#include "../common/log.h"
#include "../common/version.h"
#include "../common/timer.h"


#include "WorldServer/WorldServer.h"

std::map<uint16_t, uint16_t> EQOpcodeVersions;

int main(int argc, char **argv)
{
	bool success = true;
	bool looping = true; // Need to move this out at some point


	LogStart();
	LogSetPrefix("EQ2Emu-WorldServer");

	WorldServer s;

	LogInfo(LOG_INIT, 0, "Starting %s v%d.%d %s", EQ2_NAME, EQ2_VERSION_MAJOR, EQ2_VERSION_MINOR, EVE_VERSION_PHASE);

	success = s.Open(9100, false);

	while (success && looping) {
		Timer::SetCurrentTime();

		success = s.Process();
		success = s.ProcessClientWrite();

		SleepMS(5);
	}

	LogStop();
#if defined(_WIN32)
	printf("Press any key to continue...");
	fgetc(stdin);
#endif
}