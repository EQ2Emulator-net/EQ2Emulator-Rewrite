#include "stdafx.h"

#include "../common/ConfigReader.h"
#include "../common/util.h"
#include "WorldTalk/WorldTalk.h"
#include "../common/timer.h"


//REMOVE
#include "../WorldServer/Database/WorldDatabase.h"
#include "../common/Classes.h"

WorldDatabase database;
Classes classes;

int main() {
	bool looping = true;

	WorldTalk talk;

	ConfigReader cr(nullptr, nullptr, &talk);
	cr.ReadConfig("zone-config.xml");

	talk.Open();

	Timer reconnectTimer;

	while (looping) {
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

		SleepMS(5);
	}

	return 0;
}